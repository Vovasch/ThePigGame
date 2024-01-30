#include "TGoToEatTask.h"
#include "TaskDispatcher.h"
#include "../Pig.h"
#include "../PigAI/PigAIController.h"
#include "../../Farm/Farm.h"
#include "../../Farm/EatingSpot.h"
#include "../PigStateMachine/PigStateMachine.h"



TGoToEatTask::TGoToEatTask() {
	m_xTaskType = ETaskType::GoToEat;
}

void TGoToEatTask::Start() {
	auto aiController = GetAIController();

	auto targetEatingSpot = GetFarm()->GetAvailableEatingSpot();
	aiController->SetTargetEatingSpot(targetEatingSpot);
	if(!targetEatingSpot) {
		OnNoEatingSpotAvailable();
		return;
	}

	aiController->Subscribe(this, EPigAIControllerEvent::CanStartEating, [this, aiController]() {
		GetPig()->StartEating();
		GetAIController()->Unsibscribe(this);
		this->Complete();
	});

	aiController->Subscribe(this, EPigAIControllerEvent::UnableToStartEating, [this, aiController]() {
		this->RestartTask();
	});

	aiController->Subscribe(this, EPigAIControllerEvent::FailedToReachEatingSpot, [this, aiController]() {
		this->RestartTask();
	});


	aiController->MoveToCurrentTargetLocation(targetEatingSpot->GetLocation(), ETargetLocationTypes::EatingSpot);
	
	TBaseTask::Start();
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

void TGoToEatTask::RestartTask() {
	GetAIController()->Unsibscribe(this);
	Fail();
	GetPig()->AddTask(ETaskType::GoToEat);
}
