#include "EatingController.h"

#include "ThePigGame/Farm/EatingSpot.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/WeightController/WeightController.h"
#include "ThePigGame/Pig/Tasks/Base/TaskDispatcher.h"

void UEatingController::Tick(float delta) {
	Super::Tick(delta);
	CheckBellyfulLevel();
}

void UEatingController::StartEating() {
	check(CurrentEatingSpot.IsValid());
	CurrentEatingSpot->SetAvailable(false);

	auto delta = m_pOwnerController->GetWeightController()->GetWeightDeltaPerTick();
	m_xBellyful.SetDelta(delta);

	GetStateMachine()->TryChangeState(EPigStates::Eating);
}

void UEatingController::EndEating() {
	check(CurrentEatingSpot.IsValid());
	CurrentEatingSpot->SetAvailable(true);

	auto delta = m_pOwnerController->GetWeightController()->GetWeightDeltaPerTick();
	m_xBellyful.SetDelta(-delta);

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

void UEatingController::CheckBellyfulLevel() {

	auto pig = GetPig();

	if(GetStateMachine()->GetCurrentStateType()==EPigStates::Eating && m_xBellyful.GetCurrent() >= GetInitData()->BellyfulLevelToStopEating) {
		EndEating();
		return;
	}

	if(m_xBellyful.GetCurrent() <= pig->GetInitData()->BellyfulLevelToWantToEat && GetStateMachine()->GetCurrentStateType() != EPigStates::Eating && !m_bIsWaitingForEatingSpot) {
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

void UEatingController::InitProperties() {
	m_xBellyful.Init(this);
	m_xBellyful.GetMinMaxType().SetMinMax(GetInitData()->MinBellyful, GetInitData()->MaxBellyful);

	auto delta = m_pOwnerController->GetWeightController()->GetWeightDeltaPerTick();

	m_xBellyful.SetDelta(-delta);
	m_xBellyful.GetCurrentModifycationType().Set(100.f);
}


const Bellyful* UEatingController::GetBellyfull() {
	return &m_xBellyful;
}