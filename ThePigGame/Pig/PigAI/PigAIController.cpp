// Fill out your copyright notice in the Description page of Project Settings.


#include "PigAIController.h"
#include "CoreMinimal.h"
#include "../Pig.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../PigStateMachine/PigStateMachine.h"
#include "../Tasks/Base/TaskDispatcher.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "../../Levels/MainLevel/MainScreenGameMode.h"
#include "ThePigGame/Pig/Movement/MovementController.h"

DEFINE_LOG_CATEGORY_STATIC(AIControllerLog, Log, All)

namespace NPigBlackBoardKeys {
	const FName PigState = TEXT("PigState");
	const FName PigTask = TEXT("PigTask");
}

APigAIController::APigAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

}

void APigAIController::Init() {
	// update enum variable "PigState" in blackboard
	GetStateMachine()->Subscribe(this, EStateMachineEvent::StateChanged, [this](EPigStates oldState, EPigStates newState) {
		SetPigState(newState);
	});

	auto taskDispatcher = GetTaskDispatcher();

	// update enum variable "PigTask" in blackboard
	taskDispatcher->Subscribe(this, ETaskDispatcherEvent::TaskStarted, [this](ETaskType taskType) {
		SetPigTask(taskType);
	});

	taskDispatcher->Subscribe(this, ETaskDispatcherEvent::TaskFinished, [this](ETaskType taskType) {
		SetPigTask(ETaskType::None);
	});
}

void APigAIController::SetPigState(EPigStates pigState) {
	//Blackboard->SetValueAsEnum(NPigBlackBoardKeys::PigState, (uint8)pigState);
}

void APigAIController::SetPigTask(ETaskType pigTask) {
	//Blackboard->SetValueAsEnum(NPigBlackBoardKeys::PigTask, (uint8)pigTask);
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

UEatingSpot* APigAIController::GetTargetEatingSpot() {
	return m_pTargetEatingSpot;
}

void APigAIController::MoveToCurrentTargetLocation(const FVector& loc, ETargetLocationTypes targetType) {
	m_xCurrentTargetLocationType = targetType;
	UE_LOG(AIControllerLog, Log, TEXT("Request move to %s. %s"), *UEnum::GetValueAsString<ETargetLocationTypes>(m_xCurrentTargetLocationType), *GetPig()->GetName());

	auto movementController = GetMovementController();

	movementController->Subscribe(this, EMovementControllerEvent::RotationFinished, [this, loc]() {
		GetMovementController()->Unsibscribe(this);
		BPMoveToCurrentTargetLocation(loc);

	});

	movementController->RotateTo(loc);
}

void APigAIController::InterruptMovement() {
	if(m_xCurrentTargetLocationType == ETargetLocationTypes::None) return;
	UE_LOG(AIControllerLog, Log, TEXT("Interrupt request of move to %s. %s"), *UEnum::GetValueAsString<ETargetLocationTypes>(m_xCurrentTargetLocationType), *GetPig()->GetName());
	BPInterruptMovement();
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
	UE_LOG(AIControllerLog, Log, TEXT("Success move to %s. %s"), *UEnum::GetValueAsString<ETargetLocationTypes>(m_xCurrentTargetLocationType), *GetPig()->GetName());
	OnMoveToTargetLocationFinished(true);
}

void APigAIController::OnMoveToTargetLocationFailed() {
	UE_LOG(AIControllerLog, Log, TEXT("Fail move to %s. %s"), *UEnum::GetValueAsString<ETargetLocationTypes>(m_xCurrentTargetLocationType), *GetPig()->GetName());
	OnMoveToTargetLocationFinished(false);
}

void APigAIController::OnStartedEating() {
	if(m_pTargetEatingSpot) m_pTargetEatingSpot->SetAvailable(false);
}

void APigAIController::OnFinishedEating() {
	if(m_pTargetEatingSpot) m_pTargetEatingSpot->SetAvailable(true);
}

void APigAIController::BindOnEvents() {
	auto gameMode = Cast<AMainScreenGameMode>(GetWorld()->GetAuthGameMode());

	gameMode->Subscribe(this, EGameModeEvent::NightStarted, [this]() {
		GetPig()->GoToSleep();
	});

	gameMode->Subscribe(this, EGameModeEvent::MorningStarted, [this]() {
		GetPig()->WakeUp();
	});
}

