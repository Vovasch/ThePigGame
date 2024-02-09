#include "GoToEatTask.h"
#include "../../Pig.h"
#include "../../PigAI/PigAIController.h"
#include "../../../Farm/Farm.h"
#include "../../../Farm/EatingSpot.h"
#include "../../PigStateMachine/PigStateMachine.h"

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
	if(!GetAIController()->GetTargetEatingSpot()->IsAvailable()) {
		TryGoToEatingSpot();
	}
}

void UGoToEatTask::TryGoToEatingSpot() {
	UnsubscribeAll();
	auto aiController = GetAIController();

	auto targetEatingSpot = GetFarm()->GetAvailableEatingSpot();
	aiController->SetTargetEatingSpot(targetEatingSpot);
	if(!targetEatingSpot) {
		OnNoEatingSpotAvailable();
		return;
	}

	aiController->Subscribe(this, EPigAIControllerEvent::ReachedEatingSpot, [this, aiController]() {
		if(GetAIController()->GetTargetEatingSpot()->IsAvailable()) {
			GetPig()->StartEating();
			this->Complete();
		} else {
			TryGoToEatingSpot();
		}
		
	});

	aiController->Subscribe(this, EPigAIControllerEvent::FailedToReachEatingSpot, [this, aiController]() {
		//this->TryGoToEatingSpot();
	});

	aiController->MoveToCurrentTargetLocation(targetEatingSpot->GetLocation(), ETargetLocationTypes::EatingSpot);
}

void UGoToEatTask::OnNoEatingSpotAvailable() {
	
	GetPig()->SetWaitingForEatingSpot(true);
	GetAIController()->Unsibscribe(this);
	Fail();

	GetFarm()->Subscribe(this, EFarmEvent::EatingSpotFreed, [this]() {
		GetFarm()->Unsibscribe(this);
		GetPig()->SetWaitingForEatingSpot(false);
		GetPig()->AddTask(ETaskType::GoToEat);
	});
}

void UGoToEatTask::UnsubscribeAll() {
	GetAIController()->Unsibscribe(this);
}
