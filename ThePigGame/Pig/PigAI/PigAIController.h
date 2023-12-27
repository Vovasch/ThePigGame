// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../PigStateMachine/PigStates.h"
#include "../Tasks/TaskType.h"
#include "../TargetLocationTypes.h"
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
	void InitAIController(UPigStateMachine* pigStateMachine, UTaskDispatcher* pigTaskDispatcher);

	public:
	virtual APig* GetPig() override;

	public:
	void SetTargetLocation(const FVector& loc, ETargetLocationTypes targetType);
	void SetTargetEatingSpot(UEatingSpot* eatingSpot);

	public:
	void OnStartedEating();
	void OnFinishedEating();

	protected:
	bool CanStartEating();

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

	UFUNCTION(BlueprintCallable)
	void OnMoveToTargetLocationFailed();

	protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector m_xTargetLocation = FVector::ZeroVector;

	protected:
	ETargetLocationTypes m_xTargetLocationType = ETargetLocationTypes::None;

	protected:
	UEatingSpot* m_pTargetEatingSpot = nullptr;
		
};
