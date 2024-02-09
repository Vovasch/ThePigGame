#include "GoToSleepTask.h"
#include "../../../Pig/Pig.h"
#include "../../../Farm/Farm.h"
#include "../../../Farm/SleepingArea/SleepingArea.h"
#include "../../../Pig/PigAI/PigAIController.h"
#include "../../../Pig/PigStateMachine/PigStateMachine.h"
#include "../../../Pig/PigStateMachine/PigSleepingState.h"
#include "Math/Box.h"
#include "../../../Utils/Misc/TMiscUtils.h"

UGoToSleepTask::UGoToSleepTask() {
	m_xTaskType = ETaskType::GoToSleep;
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

	GetAIController()->Unsibscribe(this);
	UBaseTask::OnEnd();
}

void UGoToSleepTask::Tick(float delta) {
	UBaseTask::Tick(delta);
	
	if(!m_pAnotherSleepingPig.IsValid()) {
		FindPlaceForSleeping();
	}
}

void UGoToSleepTask::FindPlaceForSleeping() {
	auto sleepingArea = GetFarm()->GetSleepingArea();
	m_pAnotherSleepingPig = sleepingArea->GetAnySleepingPig();
	
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

	} else if(m_xSleepingSpotType==ESleepingSpotType::None){

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

	auto aiController = GetAIController();	

	aiController->MoveToCurrentTargetLocation(moveToLocation, ETargetLocationTypes::SleepingSpot);

	aiController->Subscribe(this, EPigAIControllerEvent::ReachedSleepingSpot, [this]() {
		Complete();
	});

	aiController->Subscribe(this, EPigAIControllerEvent::FailedToReachSleepingSpot, [this]() {
		//OnFailedToReachSleepingPlace();
	});

}

void UGoToSleepTask::OnAnotherSleepingPigEndedSleeping() {
	UnsubscribeFromAnotherSleepingPig();

	FindPlaceForSleeping();

}

void UGoToSleepTask::UnsubscribeFromAnotherSleepingPig() {
	if(m_pAnotherSleepingPig.IsValid()) {
		m_pAnotherSleepingPig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Unsibscribe(this);
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
		// if so this pig should make load sound. than other pigs will changed their positions of sleeping
		// to let this pig get onto sleeping area.
		FindPlaceForSleeping();
	}
}
