#include "EatingController.h"

#include "ThePigGame/Farm/Components/Trough/EatingSpot.h"
#include "ThePigGame/Farm/Components/Trough/Trough.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/WeightController/WeightController.h"
#include "ThePigGame/Pig/Tasks/Base/TaskDispatcher.h"

void UEatingController::Tick(float delta) {
	Super::Tick(delta);

	if(GetStateMachine()->GetCurrentStateType() == EPigStates::Eating) {
		ProcessEatingState();		
	} else {
		ProcessNonEatingState();
	}
}

void UEatingController::StartEating() {
	check(CurrentEatingSpot.IsValid());
	CurrentEatingSpot->SetAvailable(false);

	GetStateMachine()->TryChangeState(EPigStates::Eating);
}

void UEatingController::EndEating() {
	check(CurrentEatingSpot.IsValid());
	CurrentEatingSpot->SetAvailable(true);
	CurrentEatingSpot = nullptr;

	GetStateMachine()->TryChangeState(EPigStates::Default);
}

UEatingSpot* UEatingController::GetTargetEatingSpot() {
	return CurrentEatingSpot.Get();
}

void UEatingController::SetTargetEatingSpot(UEatingSpot* EatingSpot) {
	CurrentEatingSpot = EatingSpot;
}

void UEatingController::SetWaitingForEatingSpot(bool isWaiting) {
	m_bIsWaitingForEatingSpot = isWaiting;
}

void UEatingController::ProcessNonEatingState() {
	auto pig = GetPig();

	m_xBellyful.GetCurrentModifycationType().Add(-GetInitData()->BellyfullLosePerTick);

	if(m_xBellyful.GetCurrent() >= pig->GetInitData()->BellyfulLevelToWantToEat || m_bIsWaitingForEatingSpot) {
		auto taskDispatcher = GetTaskDispatcher();

		auto task = taskDispatcher->GetTaskByType(ETaskType::GoToEat);

		task->Subscribe(this, ETaskEvent::Success, [this]() {
			GetTaskDispatcher()->GetTaskByType(ETaskType::GoToEat)->Unsubscribe(this);
			StartEating();
		});

		task->Subscribe(this, ETaskEvent::Fail, [this]() {
			GetTaskDispatcher()->GetTaskByType(ETaskType::GoToEat)->Unsubscribe(this);			
		});

		taskDispatcher->AddTask(ETaskType::GoToEat);
	}
}

void UEatingController::ProcessEatingState() {

	auto amountToEat = GetInitData()->EatDeltaPerTick;
	auto success = CurrentEatingSpot->GetOwnerTrough()->TryEatOut(amountToEat);

	if(success) {
		m_xBellyful.GetCurrentModifycationType().Add(amountToEat);
	} else {
		EndEating();
		return;
	}

	if(m_xBellyful.GetCurrent() >= GetInitData()->BellyfulLevelToStopEating) {
		EndEating();
	}

}

void UEatingController::InitProperties() {
	m_xBellyful.Init(this);
	m_xBellyful.GetMinMaxType().SetMinMax(GetInitData()->MinBellyful, GetInitData()->MaxBellyful);

	m_xBellyful.GetCurrentModifycationType().Set(100.f);
}


const Bellyful* UEatingController::GetBellyfull() {
	return &m_xBellyful;
}