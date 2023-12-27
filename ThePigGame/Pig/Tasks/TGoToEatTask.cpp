#include "TGoToEatTask.h"
#include "TaskDispatcher.h"
#include "../Pig.h"
#include "../PigAI/PigAIController.h"
#include "../../Farm/Farm.h"
#include "../../Farm/EatingSpot.h"



TGoToEatTask::TGoToEatTask(UTaskDispatcher* owner) : TBaseTask(owner) {
	m_xTaskType = ETaskType::GoToEat;
}

void TGoToEatTask::Start() {
	auto pig = m_pTaskDispatcherOwner->GetPigOwner();
	auto aiController = pig->GetPigAIController();
	auto farm = pig->GetOwnerFarm();

	auto targetEatingSpot = farm->GetAvailableEatingSpot();
	aiController->SetTargetEatingSpot(targetEatingSpot);
	if(!targetEatingSpot) {
		Fail();
		return;
	}

	aiController->Subscribe(this, EPigAIControllerEvent::ReachedEatingSpot, [this, aiController]() {
		this->Complete();
		aiController->Unsibscribe(this);
	});

	aiController->SetTargetLocation(targetEatingSpot->GetLocation(), ETargetLocationTypes::EatingSpot);
	
	TBaseTask::Start();

}

void TGoToEatTask::Fail() {
	TBaseTask::Fail();
	
	auto pig = m_pTaskDispatcherOwner->GetPigOwner();
	auto aiController = pig->GetPigAIController();
	auto farm = pig->GetOwnerFarm();

	farm->Subscribe(this, EFarmEvent::EatingSpotFreed, [this, pig, farm]() {
		pig->AddTask(ETaskType::GoToEat);
		farm->Unsibscribe(this);
	});



}
