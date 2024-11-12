#include "GoToSleepTask.h"
#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Farm/Controllers/SleepingPigsController/SleepingPigsController.h"
#include "ThePigGame/Pig/Movement/MovementController.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/PigStateMachine/PigStates.h"
#include "ThePigGame/Utils/Misc/TMiscUtils.h"
#include "ThePigGame/Utils/StateMachine/StateEvent.h"


ETaskType UGoToSleepTask::GetTaskType() {
	return ETaskType::GoToSleep;
}

void UGoToSleepTask::Start() {
	UBaseTask::Start();
	FindPlaceForSleeping();
}

void UGoToSleepTask::Complete() {
	GetStateMachine()->TryChangeState(EPigStates::LayingDown);
	UBaseTask::Complete();
}

void UGoToSleepTask::OnEnd() {
	UnsubscribeFromAnotherSleepingPig();

	GetMovementController()->Unsubscribe(this);
	UBaseTask::OnEnd();
}

void UGoToSleepTask::Tick(float delta) {
	if(!m_pAnotherSleepingPig.IsValid()) {
		FindPlaceForSleeping();
	}
}

void UGoToSleepTask::FindPlaceForSleeping() {
	
	m_pAnotherSleepingPig = GetFarm()->GetSleepingPigsController()->GetAnySleepingPig();
	
	auto moveToLocation = FVector::ZeroVector;

	if(m_pAnotherSleepingPig.IsValid()) {
		m_pAnotherSleepingPig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Subscribe(this, EStateEvent::End, [this]() {
			this->OnAnotherSleepingPigEndedSleeping();
		});

		auto thisPigExtent = utils::GetActorExtent(GetPig());
		auto anotherPigExtent = utils::GetActorExtent(m_pAnotherSleepingPig.Get());

		thisPigExtent += anotherPigExtent;
		thisPigExtent.Z = 0;

	/*	auto cc = m_pAnotherSleepingPig->GetActorLocation();
		auto cc2 = thisPigExtent.Length();*/


		moveToLocation = utils::CutSegment2D(GetPig()->GetActorLocation(), m_pAnotherSleepingPig->GetActorLocation(), thisPigExtent.Length());
		//auto cc3 = FVector::Dist2D(cc, moveToLocation);

		m_xSleepingSpotType = ESleepingSpotType::AnotherPig;

	} else if(m_xSleepingSpotType==ESleepingSpotType::None) {
		auto sleepingArea = GetFarm()->GetSleepingArea();

		static const auto extent = sleepingArea->GetStaticMesh()->GetBounds().BoxExtent / 2;
		static const auto sleepingAreaLocation = sleepingArea->GetComponentLocation();
		static const auto box = FBox(sleepingAreaLocation - extent, sleepingAreaLocation + extent);

		auto pigExtent = utils::GetActorExtent(GetPig());

		auto shrinkedBox = box;
		shrinkedBox.Min += pigExtent;
		shrinkedBox.Max -= pigExtent;

		moveToLocation = FMath::RandPointInBox(shrinkedBox);
		m_xSleepingSpotType = ESleepingSpotType::RandomPlace;
	} else {
		return;
	}

	auto movementController = GetMovementController();	

	movementController->MoveTo(moveToLocation);

	movementController->Subscribe(this, EMovementControllerEvent::MovementCompleted, [this]() {
		Complete();
	});

	movementController->Subscribe(this, EMovementControllerEvent::MovementFailed, [this]() {
		// TODO: handle failed to move 
		//OnFailedToReachSleepingPlace();
	});

}

void UGoToSleepTask::OnAnotherSleepingPigEndedSleeping() {
	UnsubscribeFromAnotherSleepingPig();

	FindPlaceForSleeping();

}

void UGoToSleepTask::UnsubscribeFromAnotherSleepingPig() {
	if(m_pAnotherSleepingPig.IsValid()) {
		m_pAnotherSleepingPig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Unsubscribe(this);
	}

	m_pAnotherSleepingPig = nullptr;
	m_xSleepingSpotType = ESleepingSpotType::None;
}

void UGoToSleepTask::OnFailedToReachSleepingPlace() {
	auto pig = GetPig();
	auto sleepingArea = pig->GetOwnerFarm()->GetSleepingArea();

	auto box = FBox();
	box = box.TransformBy(sleepingArea->GetComponentTransform());

	if(box.IsInside(pig->GetActorLocation())) {
		Complete();
	} else {

		// TODO: here we check if pig can't get onto sleeping area because of other pigs
		// if so this pig should make load sound. then other pigs will change their positions of sleeping
		// to let this pig get onto sleeping area.
		FindPlaceForSleeping();
	}
}
