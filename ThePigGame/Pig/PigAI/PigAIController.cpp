// Fill out your copyright notice in the Description page of Project Settings.


#include "PigAIController.h"
#include "CoreMinimal.h"
#include "../Pig.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../PigStateMachine/PigStateMachine.h"
#include "../Tasks/TaskDispatcher.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "../../Levels/MainLevel/MainScreenGameMode.h"

namespace NPigBlackBoardKeys {
	const FName PigState = TEXT("PigState");
	const FName PigTask = TEXT("PigTask");
}

APigAIController::APigAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

}

void APigAIController::Init() {
	// update enum variable "PigState" in blackboard
	GetStateMachine()->Subscribe(EStateMachineEvent::StateChanged, [this](EPigStates oldState, EPigStates newState) {
		SetPigState(newState);
	});

	auto taskDispatcher = GetTaskDispatcher();

	// update enum variable "PigTask" in blackboard
	taskDispatcher->Subscribe(ETaskDispatcherEvent::TaskStarted, [this](ETaskType taskType) {
		SetPigTask(taskType);
	});

	taskDispatcher->Subscribe(ETaskDispatcherEvent::TaskFinished, [this](ETaskType taskType) {
		SetPigTask(ETaskType::None);
	});
}

void APigAIController::SetPigState(EPigStates pigState) {
	Blackboard->SetValueAsEnum(NPigBlackBoardKeys::PigState, (uint8)pigState);
}

void APigAIController::SetPigTask(ETaskType pigTask) {
	Blackboard->SetValueAsEnum(NPigBlackBoardKeys::PigTask, (uint8)pigTask);
}

void APigAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	auto pigPawn = Cast<APig>(InPawn);
	pigPawn->SetPigAIController(this);
	BindOnEvents();
}

APig* APigAIController::GetPig() {
	return Cast<APig>(GetPawn());
}

void APigAIController::SetTargetEatingSpot(UEatingSpot* eatingSpot) {
	m_pTargetEatingSpot = eatingSpot;
}

void APigAIController::MoveToCurrentTargetLocation(const FVector& loc, ETargetLocationTypes targetType) {
	++m_uMoveRequestCounter;
	BPMoveToCurrentTargetLocation(loc, targetType);
}

void APigAIController::OnTargetLocationEvent(ETargetLocationTypes targetType, bool success) {
	--m_uMoveRequestCounter;
	if(m_uMoveRequestCounter != 0) return;

	if(targetType == ETargetLocationTypes::EatingSpot) {
		OnEvent(success ? EPigAIControllerEvent::ReachedEatingSpot : EPigAIControllerEvent::FailedToReachEatingSpot);
	} else if(targetType == ETargetLocationTypes::SleepingSpot) {
		OnEvent(success ? EPigAIControllerEvent::ReachedSleepingSpot : EPigAIControllerEvent::FailedToReachSleepingSpot);
	}
}


void APigAIController::OnTargetLocationReached(ETargetLocationTypes targetType) {
	OnTargetLocationEvent(targetType, true);
}

void APigAIController::OnMoveToTargetLocationFailed(ETargetLocationTypes targetType) {
	OnTargetLocationEvent(targetType, false);
}

bool APigAIController::CanStartEating() {
	return m_pTargetEatingSpot && m_pTargetEatingSpot->IsAvailable();
}

void APigAIController::OnStartedEating() {
	if(m_pTargetEatingSpot) m_pTargetEatingSpot->SetAvailable(false);
}

void APigAIController::OnFinishedEating() {
	if(m_pTargetEatingSpot) m_pTargetEatingSpot->SetAvailable(true);
}


void APigAIController::BindOnEvents() {
	Subscribe(EPigAIControllerEvent::ReachedEatingSpot, [this]() {
		if (CanStartEating()) {
			OnEvent(EPigAIControllerEvent::CanStartEating);
		} else {
			OnEvent(EPigAIControllerEvent::UnableToStartEating);
		}
	});

	auto gameMode = Cast<AMainScreenGameMode>(GetWorld()->GetAuthGameMode());

	gameMode->Subscribe(this, EGameModeEvent::NightStarted, [this]() {
		GetPig()->GoToSleep();
	});

	gameMode->Subscribe(this, EGameModeEvent::MorningStarted, [this]() {
		GetPig()->WakeUp();
	});


}

