// Fill out your copyright notice in the Description page of Project Settings.


#include "PigAIController.h"
#include "CoreMinimal.h"
#include "../Pig.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../PigStateMachine/PigStateMachine.h"
#include "../Tasks/TaskDispatcher.h"

namespace NPigBlackBoardKeys {
	const FName PigState = TEXT("PigState");
	const FName PigTask = TEXT("PigTask");
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


void APigAIController::SetTargetLocation(const FVector& loc, ETargetLocationTypes targetType) {
	if (loc == FVector::ZeroVector) {
		return;
	}
	m_xTargetLocation = loc;
	m_xTargetLocationType = targetType;
}

void APigAIController::SetTargetEatingSpot(UEatingSpot* eatingSpot) {
	m_pTargetEatingSpot = eatingSpot;
}

void APigAIController::OnTargetLoacationEvent(bool success) {
	if(m_xTargetLocationType == ETargetLocationTypes::EatingSpot) {
		OnEvent(success ? EPigAIControllerEvent::ReachedEatingSpot : EPigAIControllerEvent::FailedToReachEatingSpot);
	} else if(m_xTargetLocationType == ETargetLocationTypes::SleepingSpot) {
		OnEvent(success ? EPigAIControllerEvent::ReachedSleepingSpot : EPigAIControllerEvent::FailedToReachSleepingSpot);
	}

	// temp comented, will be uncomented once custom move system will be designed
	//m_xTargetLocation = FVector::ZeroVector;
	//m_xTargetLocationType = ETargetLocationTypes::None;

}


void APigAIController::OnTargetLocationReached() {
	OnTargetLoacationEvent(true);
}

void APigAIController::OnMoveToTargetLocationFailed() {
	OnTargetLoacationEvent(false);
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
}

