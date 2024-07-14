#include "GoToEatTask.h"
#include "../../PigAI/PigAIController.h"
#include "../../../Farm/Farm.h"
#include "../../../Farm/EatingSpot.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/EatingController/EatingController.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"
#include "ThePigGame/Pig/Tasks/Base/TaskDispatcher.h"

UGoToEatTask::UGoToEatTask() {
	m_xTaskType = ETaskType::GoToEat;
}

void UGoToEatTask::Start() {
	UBaseTask::Start();
	TryGoToEatingSpot();
}

void UGoToEatTask::OnEnd() {
	UnsubscribeAll();
	UBaseTask::OnEnd();
}

void UGoToEatTask::Tick(float delta) {
	UBaseTask::Tick(delta);

	// while going to eating spot another pig took our eating spot
	// so we try to find new one
	if(!GetPropertyController()->GetEatingController()->GetTargetEatingSpot()->IsAvailable()) {
		TryGoToEatingSpot();
	}
}

void UGoToEatTask::TryGoToEatingSpot() {
	UnsubscribeAll();
	auto aiController = GetAIController();

	auto targetEatingSpot = GetFarm()->GetAvailableEatingSpot();
	GetPropertyController()->GetEatingController()->SetTargetEatingSpot(targetEatingSpot);
	if(!targetEatingSpot) {
		OnNoEatingSpotAvailable();
		return;
	}

	aiController->Subscribe(this, EPigAIControllerEvent::ReachedEatingSpot, [this, aiController]() {
		if(GetPropertyController()->GetEatingController()->GetTargetEatingSpot()->IsAvailable()) {
			this->Complete();
		} else {
			TryGoToEatingSpot();
		}
		
	});

	aiController->Subscribe(this, EPigAIControllerEvent::FailedToReachEatingSpot, [this, aiController]() {
		// TODO: handle failed to move
		//this->TryGoToEatingSpot();
	});

	aiController->MoveToTargetLocation(targetEatingSpot->GetLocation(), ETargetLocationTypes::EatingSpot);
}

void UGoToEatTask::OnNoEatingSpotAvailable() {
	
	GetPropertyController()->GetEatingController()->SetWaitingForEatingSpot(true);
	GetAIController()->Unsubscribe(this);
	Fail();

	GetFarm()->Subscribe(this, EFarmEvent::EatingSpotFreed, [this]() {
		GetFarm()->Unsubscribe(this);

		GetPropertyController()->GetEatingController()->SetWaitingForEatingSpot(false);
		GetTaskDispatcher()->AddTask(ETaskType::GoToEat);
	});
}

void UGoToEatTask::UnsubscribeAll() {
	GetAIController()->Unsubscribe(this);
}
