// Fill out your copyright notice in the Description page of Project Settings.


#include "PigAIController.h"
#include "CoreMinimal.h"
#include "../Pig.h"
#include "../PigStateMachine/PigStateMachine.h"
#include "../Tasks/Base/TaskDispatcher.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "ThePigGame/Pig/Movement/MovementController.h"

DEFINE_LOG_CATEGORY_STATIC(AIControllerLog, Log, All)

namespace NPigBlackBoardKeys {
	const FName PigState = TEXT("PigState");
	const FName PigTask = TEXT("PigTask");
}

APigAIController::APigAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

}

APig* APigAIController::GetPig() {
	return Cast<APig>(GetPawn());
}


void APigAIController::MoveToTargetLocation(const FVector& loc, ETargetLocationTypes targetType) {
	m_xCurrentTargetLocationType = targetType;
	UE_LOG(AIControllerLog, Log, TEXT("Request move to %s. %s"), *UEnum::GetDisplayValueAsText(m_xCurrentTargetLocationType).ToString(), *GetPig()->GetName());

	auto movementController = GetMovementController();

	movementController->Unsubscribe(this);

	movementController->Subscribe(this, EMovementControllerEvent::MovementCompleted, [this]() {
		GetMovementController()->Unsubscribe(this);
		OnTargetLocationReached();

	});

	movementController->Subscribe(this, EMovementControllerEvent::MovementFailed, [this]() {
		GetMovementController()->Unsubscribe(this);
		OnMoveToTargetLocationFailed();
	});

	movementController->MoveTo(loc);
}

void APigAIController::InterruptMovement() {
	if(m_xCurrentTargetLocationType == ETargetLocationTypes::None) return;
	UE_LOG(AIControllerLog, Log, TEXT("Interrupt request of move to %s. %s"), *UEnum::GetDisplayValueAsText(m_xCurrentTargetLocationType).ToString(), *GetPig()->GetName());

	GetMovementController()->InterruptMovement();
}

void APigAIController::OnMoveToTargetLocationFinished(bool success) {
	auto copy = m_xCurrentTargetLocationType;

	// i have to clear it before event
	// OnEvent can be created new move request
	// if i will set None after all OnEvent's
	// i can override existing
	m_xCurrentTargetLocationType = ETargetLocationTypes::None;
	if(copy == ETargetLocationTypes::EatingSpot) {
		OnEvent(success ? EPigAIControllerEvent::ReachedEatingSpot : EPigAIControllerEvent::FailedToReachEatingSpot);
	} else if(copy == ETargetLocationTypes::SleepingSpot) {
		OnEvent(success ? EPigAIControllerEvent::ReachedSleepingSpot : EPigAIControllerEvent::FailedToReachSleepingSpot);
	} else if(copy == ETargetLocationTypes::RandomLocation) {
		OnEvent(success ? EPigAIControllerEvent::ReachedRandomLocation : EPigAIControllerEvent::FailedToReachRandomLocation);
	}
}


void APigAIController::OnTargetLocationReached() {
	UE_LOG(AIControllerLog, Log, TEXT("Success move to %s. %s"), *UEnum::GetDisplayValueAsText(m_xCurrentTargetLocationType).ToString(), *GetPig()->GetName());
	OnMoveToTargetLocationFinished(true);
}

void APigAIController::OnMoveToTargetLocationFailed() {
	UE_LOG(AIControllerLog, Log, TEXT("Fail move to %s. %s"), *UEnum::GetDisplayValueAsText(m_xCurrentTargetLocationType).ToString(), *GetPig()->GetName());
	OnMoveToTargetLocationFinished(false);
}



