#include "ConsumingController.h"
#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Farm/Components/ConsumeSpotComponent/ConsumeSpotComponent.h"
#include "ThePigGame/Farm/Controllers/ConsumeSpotsController/ConsumeSpotsController.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/WeightController/WeightController.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskDispatcher/TaskDispatcher.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskHelper/ConsumeConnector.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Base/TaskBase.h"

DEFINE_LOG_CATEGORY_STATIC(ConsumingControllerLog, Log, All)

UConsumingController::UConsumingController() {

	m_mConsumeProperties.Add( EConsumeSourceType::Eating, {});
	m_mConsumeProperties.Add( EConsumeSourceType::Drinking, {} );

	if(m_mConsumeProperties.Num()!=uint32(EConsumeSourceType::Size)) {
		UE_LOG(ConsumingControllerLog, Fatal, TEXT("Amount of consuming properties isn't equal to the amount of consuming spots"));
	}
}

void UConsumingController::Tick(float delta) {
	UPropertySubControllerBase::Tick(delta);
	ProcessTick();
}

EConsumeSourceType UConsumingController::GetOccupiedSpotTypeChecked() {
	if(!m_pOccupiedSpot.IsValid()) {
		UE_LOG(ConsumingControllerLog, Fatal, TEXT("Pig is not in consuming state or did't occupiy consume spot"));
	}
	return m_pOccupiedSpot->GetSpotType();
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
	/// todo WorldCharacteristic but in init data for pig we should ensure that pig would be eating faster than loosing bellyful

	auto& consumingData = GetInitData()->ConsumingData;

	for(auto& [sourceType, property] : m_mConsumeProperties) {

		property.GetCurrentModifycationType().Add(-consumingData[sourceType].LoseDeltaPerTick);

		auto& currentConsumingData = consumingData[sourceType];
		if(property.GetCurrent() <= currentConsumingData.AmountToWantToConsume
			&& !m_vWaitingForSpot[uint32(sourceType)]
			&& (!m_pOccupiedSpot.IsValid() || m_pOccupiedSpot->GetSpotType()!=sourceType)) 
		{
			AddGoToConsumeSpotTask(sourceType);
		}
	}

	if(GetStateMachine()->GetCurrentStateType()==EPigStates::Consuming) {
		ProcessConsumingState();
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

ConsumeProperty& UConsumingController::GetPropertyByConsumeType(EConsumeSourceType sourceType) {
	return m_mConsumeProperties[sourceType];
}

void UConsumingController::InitProperties() {
	auto initData = GetInitData();

	for(auto& [sourceType, property] : m_mConsumeProperties) {

		property.Init(this);
		
		auto& consumeData = initData->ConsumingData[sourceType];

		property.GetMinMaxType().SetMinMax(consumeData.Min, consumeData.Max);
		property.GetCurrentModifycationType().Set(consumeData.StartingValue);
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
	TFunction<void()> onRetryToConsumeLater{[this, sourceType]() {
		m_vWaitingForSpot[uint32(sourceType)] = false;
		AddGoToConsumeSpotTask(sourceType);
	}};

	GetPig()->GetWorldTimerManager().SetTimer(h,
		MoveTemp(onRetryToConsumeLater),
		1.f,
		false,
		GetInitData()->ConsumingData[sourceType].WaitUntilRetryConsume);
}

void UConsumingController::AddGoToConsumeSpotTask(EConsumeSourceType consumeType) {
	auto taskDispatcher = GetTaskDispatcher();
	auto taskType = ConsumeConnector::TaskBySource(consumeType);
	auto goToConsumeTask = taskDispatcher->GetTaskByType(taskType);

	// No subscribe to success here
	// because StartConsuming function will be explicitly called from inside the task

	goToConsumeTask->Subscribe(this, ETaskEvent::Fail, [this, consumeType]() {
		TryConsumeLater(consumeType);
	});

	GetTaskDispatcher()->AddTask(taskType);
}

const ConsumeProperty* UConsumingController::GetBellyful() {
	return &GetPropertyByConsumeType(EConsumeSourceType::Eating);
}

const ConsumeProperty* UConsumingController::GetHydrated() {
	return &GetPropertyByConsumeType(EConsumeSourceType::Drinking);
}
