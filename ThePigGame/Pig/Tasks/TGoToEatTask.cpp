#include "TGoToEatTask.h"
#include "TaskDispatcher.h"
#include "../Pig.h"
#include "../PigAI/PigAIController.h"
#include "../../Farm/Farm.h"
#include "../../Farm/EatingSpot.h"



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

	aiController->Subscribe(this, EPigAIControllerEvent::ReachedEatingSpot, [this, aiController]() {
		this->Complete();
		aiController->Unsibscribe(this);
	});

	aiController->Subscribe(this, EPigAIControllerEvent::FailedToReachEatingSpot, [this, aiController]() {
		this->OnFailedToReachEatingSpot();
		aiController->Unsibscribe(this);
	});

	aiController->SetTargetLocation(targetEatingSpot->GetLocation(), ETargetLocationTypes::EatingSpot);
	
	TBaseTask::Start();
}

void TGoToEatTask::OnNoEatingSpotAvailable() {
	
	GetFarm()->Subscribe(this, EFarmEvent::EatingSpotFreed, [this]() {
		GetPig()->AddTask(ETaskType::GoToEat);
		GetFarm()->Unsibscribe(this);
	});

	TBaseTask::Fail();

}

void TGoToEatTask::OnFailedToReachEatingSpot() {
	Fail();
	GetPig()->AddTask(ETaskType::GoToEat);
}
