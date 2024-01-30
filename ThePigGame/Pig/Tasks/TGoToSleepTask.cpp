#include "TGoToSleepTask.h"
#include "../../Pig/Pig.h"
#include "../../Farm/Farm.h"
#include "../../Farm/SleepingArea/SleepingArea.h"
#include "../../Pig/PigAI/PigAIController.h"
#include "../../Pig/PigStateMachine/PigStateMachine.h"
#include "../../Pig/PigStateMachine/PigSleepingState.h"
#include "Math/Box.h"
#include "../../Utils/Misc/TMiscUtils.h"

TGoToSleepTask::TGoToSleepTask() {
	m_xTaskType = ETaskType::GoToSleep;
}

void TGoToSleepTask::Start() {
	TBaseTask::Start();

	FindPlaceForSleeping();

}

void TGoToSleepTask::Complete() {
	GetStateMachine()->TryChangeState(EPigStates::LayingDown);
	TBaseTask::Complete();

}

void TGoToSleepTask::OnEnd() {
	TBaseTask::OnEnd();
	UnsubscribeFromAnotherSleepingPig();

	GetAIController()->Unsibscribe(this);
}

void TGoToSleepTask::Tick(float delta) {
	TBaseTask::Tick(delta);
	
	if(!m_pAnotherSleepingPig.IsValid()) {
		FindPlaceForSleeping();
	}
}

void TGoToSleepTask::FindPlaceForSleeping() {
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
		OnFailedToReachSleepingPlace();
	});

}

void TGoToSleepTask::OnAnotherSleepingPigEndedSleeping() {
	UnsubscribeFromAnotherSleepingPig();

	FindPlaceForSleeping();

}

void TGoToSleepTask::UnsubscribeFromAnotherSleepingPig() {
	if(m_pAnotherSleepingPig.IsValid()) {
		m_pAnotherSleepingPig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Unsibscribe(this);
	}

	m_pAnotherSleepingPig = nullptr;
	m_xSleepingSpotType = ESleepingSpotType::None;
}

void TGoToSleepTask::OnFailedToReachSleepingPlace() {
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
