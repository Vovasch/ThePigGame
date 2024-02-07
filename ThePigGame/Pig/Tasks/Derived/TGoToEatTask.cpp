#include "TGoToEatTask.h"
#include "../../Pig.h"
#include "../../PigAI/PigAIController.h"
#include "../../../Farm/Farm.h"
#include "../../../Farm/EatingSpot.h"
#include "../../PigStateMachine/PigStateMachine.h"

TGoToEatTask::TGoToEatTask() {
	m_xTaskType = ETaskType::GoToEat;
}

void TGoToEatTask::Start() {
	TBaseTask::Start();
	TryGoToEatingSpot();
}

void TGoToEatTask::OnEnd() {
	UnsubscribeAll();
	TBaseTask::OnEnd();
}

void TGoToEatTask::Tick(float delta) {
	TBaseTask::Tick(delta);

	// while going to eating spot another pig took our eating spot
	// so we try to find new one
	if(!GetAIController()->GetTargetEatingSpot()->IsAvailable()) {
		TryGoToEatingSpot();
	}
}

void TGoToEatTask::TryGoToEatingSpot() {
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

void TGoToEatTask::OnNoEatingSpotAvailable() {
	
	GetPig()->SetWaitingForEatingSpot(true);
	GetAIController()->Unsibscribe(this);
	Fail();

	GetFarm()->Subscribe(this, EFarmEvent::EatingSpotFreed, [this]() {
		GetFarm()->Unsibscribe(this);
		GetPig()->SetWaitingForEatingSpot(false);
		GetPig()->AddTask(ETaskType::GoToEat);
	});
}

void TGoToEatTask::UnsubscribeAll() {
	GetAIController()->Unsibscribe(this);
}
