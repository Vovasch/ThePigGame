// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/UObjectGlobals.h"
#include "../PigStateMachine/PigStates.h"
#include "../Tasks/Derived/TaskType.h"
#include "TargetLocationTypes.h"
#include "../../Utils/EventHandler/TEventHandler.h"
#include "PigAIControllerEvent.h"
#include "../../Farm/EatingSpot.h"
#include "../../Utils/PigDataUser/IPigDataUser.h"
#include "PigAIController.generated.h"

class APig;
class UTaskDispatcher;

/**
 * 
 */
UCLASS()
class THEPIGGAME_API APigAIController : public AAIController, public TEventHandler<EPigAIControllerEvent>, public INoCachePigDataUser {
	GENERATED_BODY()

	public:
	APigAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	public:
	void Init();
	virtual APig* GetPig() override;

	public:
	void SetTargetEatingSpot(UEatingSpot* eatingSpot);
	UEatingSpot* GetTargetEatingSpot();

	public:
	void OnStartedEating();
	void OnFinishedEating();

	public:
	void MoveToCurrentTargetLocation(const FVector& loc, ETargetLocationTypes targetType);
	void InterruptMovement();

	protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BPMoveToCurrentTargetLocation(const FVector& loc);

	UFUNCTION(BlueprintImplementableEvent)
	void BPInterruptMovement();

	protected:
	void SetPigState(EPigStates pigState);
	void SetPigTask(ETaskType pigTask);

	protected:
	virtual void OnPossess(APawn* InPawn) override;

	protected:
	void BindOnEvents();

	protected:
	UFUNCTION(BlueprintCallable)
	void OnTargetLocationReached();

	protected:
	UFUNCTION(BlueprintCallable)
	void OnMoveToTargetLocationFailed();

	protected:
	void OnMoveToTargetLocationFinished(bool succes);

	protected:
	UEatingSpot* m_pTargetEatingSpot = nullptr;

	protected:
	ETargetLocationTypes m_xCurrentTargetLocationType = ETargetLocationTypes::None;
};
