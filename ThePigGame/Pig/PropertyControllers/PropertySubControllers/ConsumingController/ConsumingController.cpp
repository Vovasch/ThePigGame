#include "ConsumingController.h"
#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Farm/Components/ConsumeSpotComponent/ConsumeSpotComponent.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/WeightController/WeightController.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskData/GoToConsumeSpotData.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskDispatcher/TaskDispatcher.h"

DEFINE_LOG_CATEGORY_STATIC(ConsumingControllerLog, Log, All)

void UConsumingController::Tick(float delta) {
	UPropertySubControllerBase::Tick(delta);
	ProcessTick();
}

void UConsumingController::OnNoConsumeSpotAvailable(EConsumeSourceType sourceType) {
	TryConsumeLater(sourceType);
}

TWeakObjectPtr<const UConsumeSpotComponent> UConsumingController::GetOccupiedSpot() {
	return m_pOccupiedSpot;
}

void UConsumingController::StartConsuming(TWeakObjectPtr<UConsumeSpotComponent> consumeSpot) {
	if(!m_pOccupiedSpot.IsExplicitlyNull()) {
		UE_LOG(ConsumingControllerLog, Fatal, TEXT("Previous comsumind hasn't been ended"));
	}

	// I do this here because of anim instance running in other thread
	// Anim instance uses type of spot to select animation
	// If I would do this assignment after changing state
	// Anim instance might get nullptr occupied spot and throw a fatal error
	// And if won't be able to occupy this spot a fatal error would be thrown
	m_pOccupiedSpot = consumeSpot;

	if(!GetStateMachine()->TryChangeState(EPigStates::Consuming)) {
		AddGoToConsumeSpotTask(consumeSpot->GetSpotType());
		// Reset because we already assigned it and have to reset if not started consuming
		m_pOccupiedSpot.Reset();
		return;
	}

	auto spotsController = GetPig()->GetOwnerFarm()->GetConsumeSpotsController();
	if(!spotsController->TryOccupySpot(consumeSpot.Get(), GetPig())) {
		UE_LOG(ConsumingControllerLog, Fatal, TEXT("Could not occupy spot. GoToSpotTask might be broken"))
	}
}

void UConsumingController::ProcessTick() {
	
	/// We loose bellyful even when we eat. that is ok 
	/// todo but in init data for pig we should ensure that pig would be eating faster than loosing bellyful
	
	auto& consumingData = GetInitData()->ConsumingData;
	for(uint32 i = 0; i < s_uConsumeTypesAmount; ++i) {
		auto enumType = EConsumeSourceType(i);

		GetPropertyByConsumeType(enumType).GetCurrentModifycationType().Add(-consumingData[enumType].LoseDeltaPerTick);
	}

	if(GetStateMachine()->GetCurrentStateType()==EPigStates::Consuming) {
		ProcessConsumingState();
	} else {
		ProcessNonConsumingState();
	}

}

void UConsumingController::ProcessConsumingState() {

	if(!m_pOccupiedSpot.IsValid()) {
		UE_LOG(ConsumingControllerLog, Fatal, TEXT("Occupied consuming spot is invalid"));
		EndConsuming();
		return;
	}
	const auto consumeType = m_pOccupiedSpot->GetSpotType();
	auto& consumeData = GetInitData()->ConsumingData[consumeType];

	auto consumeOutAmount = m_pOccupiedSpot->TryConsumeOut(consumeData.GainDeltaPerTick, GetPig());
	auto& consumeProperty = GetPropertyByConsumeType(consumeType);

	if(!consumeOutAmount.IsSet()) {
		if(consumeProperty.GetCurrent() < consumeData.AmountToStopConsume) {
			TryConsumeLater(m_pOccupiedSpot->GetSpotType());
		}

		EndConsuming();
		return;
	}

	consumeProperty.GetCurrentModifycationType().Add(consumeOutAmount.GetValue());

	if(consumeProperty.GetCurrent()>=consumeData.AmountToStopConsume) {
		EndConsuming();
	}
}

void UConsumingController::ProcessNonConsumingState() {

	auto& consumingData = GetInitData()->ConsumingData;

	for(uint32 i = 0; i < s_uConsumeTypesAmount; ++i) {

		auto sourceType = EConsumeSourceType(i);
		auto& currentConsumingData = consumingData[sourceType];

		if(GetPropertyByConsumeType(sourceType).GetCurrent() <= currentConsumingData.AmountToWantToConsume && !m_vWaitingForSpot[i]) {
			AddGoToConsumeSpotTask(sourceType);
		}
	}
}

Consume& UConsumingController::GetPropertyByConsumeType(EConsumeSourceType sourceType) {
	return m_vConsumeProperties[uint32(sourceType)];
}

void UConsumingController::InitProperties() {
	auto initData = GetInitData();

	for(uint32 i = 0; i < s_uConsumeTypesAmount; ++i) {
		auto& currentProperty = m_vConsumeProperties[i];
		currentProperty.Init(this);
		
		auto& consumeData = initData->ConsumingData[EConsumeSourceType(i)];

		currentProperty.GetMinMaxType().SetMinMax(consumeData.Min, consumeData.Max);
		currentProperty.GetCurrentModifycationType().Set(consumeData.StartingValue);
	}
}

void UConsumingController::EndConsuming() {
	if(!GetStateMachine()->TryChangeState(EPigStates::Default)) {
		UE_LOG(ConsumingControllerLog, Fatal, TEXT("Logical error, can't end consuming bacause can't change state to default"));
		return;
	}

	auto consumeSpotController = GetPig()->GetOwnerFarm()->GetConsumeSpotsController();
	consumeSpotController->TryFreeSpot(m_pOccupiedSpot.Get(), GetPig());
	m_pOccupiedSpot.Reset();
}

void UConsumingController::TryConsumeLater(EConsumeSourceType sourceType) {
	auto& isWaiting = m_vWaitingForSpot[uint32(sourceType)];
	if(isWaiting) {
		return;
	}

	UE_LOG(ConsumingControllerLog, Log, TEXT("Pig will try to consume later"));

	isWaiting = true;

	FTimerHandle h;
	TFunction<void()> onRetryToConsumeLayter{[this, sourceType]() {
		m_vWaitingForSpot[uint32(sourceType)] = false;
		AddGoToConsumeSpotTask(sourceType);
	}};

	// todo check if timer works correctly.
	GetPig()->GetWorldTimerManager().SetTimer(h,
		MoveTemp(onRetryToConsumetLayer),
		1.f,
		false,
		GetInitData()->ConsumingData[sourceType].WaitUntilRetryConsume);
}

void UConsumingController::AddGoToConsumeSpotTask(EConsumeSourceType consumeType) {
	auto data = TStrongObjectPtr(NewObject<UGoToConsumeSpotData>());
	data->ConsumeType = consumeType;
	GetTaskDispatcher()->AddTask(ETaskType::GoToConsumeSpot, data);
	/// todo I add task. and don't subsribe to succes
	/// because taks is calling StartConsuming and OnNoSpotAvailable
	/// this is bad because task can fail when pig for instance can't reach to spot
}

const Consume* UConsumingController::GetBellyful() {
	return &GetPropertyByConsumeType(EConsumeSourceType::Eating);
}
