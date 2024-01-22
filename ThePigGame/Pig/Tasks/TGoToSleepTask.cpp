#include "TGoToSleepTask.h"
#include "../../Pig/Pig.h"
#include "../../Farm/Farm.h"
#include "../../Farm/SleepingArea/SleepingArea.h"
#include "../../Pig/PigAI/PigAIController.h"
#include "../../Pig/PigStateMachine/PigStateMachine.h"
#include "../../Pig/PigStateMachine/PigSleepingState.h"
#include "Math/Box.h"

TGoToSleepTask::TGoToSleepTask() {
	m_xTaskType = ETaskType::GoToSleep;
}

void TGoToSleepTask::Start() {
	TBaseTask::Start();

	FindPlaceForSleeping();

}

void TGoToSleepTask::Complete() {
	TBaseTask::Complete();

	GetStateMachine()->TryChangeState(EPigStates::LayingDown);
}

void TGoToSleepTask::OnEnd() {
	TBaseTask::OnEnd();
	UnsubscribeFromAnotherSleepingPig();

	GetAIController()->Unsibscribe(this);
}

void TGoToSleepTask::Tick(float delta) {
	TBaseTask::Tick(delta);

	// TODO:
	//if(m_pAnotherSleepingPig->BecomeInvalid?
}

void TGoToSleepTask::FindPlaceForSleeping() {
	auto sleepingArea = GetPig()->GetOwnerFarm()->GetSleepingArea();
	m_pAnotherSleepingPig = sleepingArea->GetAnySleepingPig();
	
	auto moveToLocation = FVector::ZeroVector;

	if(m_pAnotherSleepingPig) {
		m_pAnotherSleepingPig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Subscribe(EStateEvent::End, [this]() {
			this->OnAnotherSleepingPigEndedSleeping();
		});

		moveToLocation = m_pAnotherSleepingPig->GetActorLocation();
	} else {

		auto box = FBox();
		// TODO: decrease box by half bounds of pig.
		moveToLocation = FMath::RandPointInBox(box.TransformBy(sleepingArea->GetComponentTransform()));
	}

	auto aiController = GetAIController();	

	aiController->MoveToCurrentTargetLocation(moveToLocation, ETargetLocationTypes::SleepingSpot);

	aiController->Subscribe(EPigAIControllerEvent::ReachedSleepingSpot, [this]() {
		Complete();
	});

	aiController->Subscribe(EPigAIControllerEvent::FailedToReachSleepingSpot, [this]() {
		OnFailedToReachSleepingPlace();
	});

}

void TGoToSleepTask::OnAnotherSleepingPigEndedSleeping() {
	UnsubscribeFromAnotherSleepingPig();

	FindPlaceForSleeping();

}

void TGoToSleepTask::UnsubscribeFromAnotherSleepingPig() {
	if(m_pAnotherSleepingPig) {
		m_pAnotherSleepingPig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Unsibscribe(this);
	}
}

void TGoToSleepTask::OnFailedToReachSleepingPlace() {
	auto pig = GetPig();
	auto sleepingArea = pig->GetOwnerFarm()->GetSleepingArea();

	auto box = FBox();
	box = box.TransformBy(sleepingArea->GetComponentTransform());

	if(box.IsInside(pig->GetActorLocation())) {
		Complete();
	} else {

		// TODO:
		// here we check if pig can't can't onto sleepng area because of other pigs
		// if so this pig should make load sound. than other pigs will changed their possitons of sleeping
		// to let this pig get onto sleeping area.
		FindPlaceForSleeping();
	}
}
