#include "GoToRandomLocationTask.h"
#include "../../Pig.h"
#include "../../Movement/MovementController.h"
#include "NavigationSystem.h"

ETaskType UGoToRandomLocationTask::GetTaskType() {
	return ETaskType::GoToRandomLocation;
}

void UGoToRandomLocationTask::Start() {
	UInterruptibleTask::Start();

	auto targetLoc = FVector::ZeroVector;
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetPig(), GetPig()->GetActorLocation(), targetLoc, 10000.f);
	
	GetMovementController()->Subscribe(this, EMovementControllerEvent::MovementCompleted, [this]() {
		this->Complete();
	});
	
	GetMovementController()->MoveTo(targetLoc);
}

void UGoToRandomLocationTask::Interrupt() {
	GetMovementController()->InterruptMovement();
	UInterruptibleTask::Interrupt();
}

void UGoToRandomLocationTask::OnEnd() {
	GetMovementController()->Unsubscribe(this);
	UBaseTask::OnEnd();
}