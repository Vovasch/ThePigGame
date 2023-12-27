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

void APigAIController::InitAIController(UPigStateMachine* pigStateMachine, UTaskDispatcher* pigTaskDispatcher) {

	// update enum variable "PigState" in blackboard
	pigStateMachine->Subscribe(EStateMachineEvent::StateChanged, [this](EPigStates oldState, EPigStates newState) {
		SetPigState(newState);
	});

	// update enum variable "PigTask" in blackboard
	pigTaskDispatcher->Subscribe(ETaskDispatcherEvent::TaskStarted, [this](ETaskType taskType) {
		SetPigTask(taskType);
	});

	pigTaskDispatcher->Subscribe(ETaskDispatcherEvent::TaskFinished, [this](ETaskType taskType) {
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
	m_xTargetLocation = loc;
	m_xTargetLocationType = targetType;
}

void APigAIController::SetTargetEatingSpot(UEatingSpot* eatingSpot) {
	m_pTargetEatingSpot = eatingSpot;
}


void APigAIController::OnTargetLocationReached() {

	if(m_xTargetLocationType == ETargetLocationTypes::EatingSpot) {
		OnEvent(EPigAIControllerEvent::ReachedEatingSpot);
	} else if(m_xTargetLocationType == ETargetLocationTypes::SleepingSpot) {
		OnEvent(EPigAIControllerEvent::ReachedSleepingSpot);
	}

	m_xTargetLocation = FVector::ZeroVector;
	m_xTargetLocationType = ETargetLocationTypes::None;
}

void APigAIController::OnMoveToTargetLocationFailed() {

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
			OnEvent(EPigAIControllerEvent::UableToStartEating);
		}
	});
}

