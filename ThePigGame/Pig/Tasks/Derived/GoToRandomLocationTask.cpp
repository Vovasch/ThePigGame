#include "GoToRandomLocationTask.h"
#include "../../PigAI/PigAIController.h"
#include "../../Pig.h"
#include "NavigationSystem.h"

UGoToRandomLocationTask::UGoToRandomLocationTask() {
	m_xTaskType = ETaskType::GoToRandomLocation;
}

void UGoToRandomLocationTask::Start() {
	UInterruptibleTask::Start();

	auto targetLoc = FVector::ZeroVector;
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetPig(), GetPig()->GetActorLocation(), targetLoc, 10000.f);
	
	GetAIController()->MoveToCurrentTargetLocation(targetLoc, ETargetLocationTypes::RandomLocation);

	GetAIController()->Subscribe(this, EPigAIControllerEvent::ReachedRandomLocation, [this]() {
		this->Complete();
	});

}

void UGoToRandomLocationTask::Interrupt() {
	GetAIController()->InterruptMovement();

	UInterruptibleTask::Interrupt();
}

void UGoToRandomLocationTask::OnEnd() {
	GetAIController()->Unsibscribe(this);
	UBaseTask::OnEnd();
}