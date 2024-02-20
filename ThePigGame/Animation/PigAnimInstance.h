// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Pig/PigStateMachine/PigStates.h"
#include "../Utils/EventHandler/TEventHandler.h"
#include "../Utils/PigDataUser/IPigDataUser.h"
#include "PigAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEPIGGAME_API UPigAnimInstance : public UAnimInstance, public TEventHandler<EPigStates>, public ICachedPigDataUser {
	GENERATED_BODY()
	
	public:
	virtual void Init(APig* pig) override;
	void DisengageAnimInstance();

	protected:
	void SetPigState(EPigStates pigState);

	protected:
	virtual void PreUpdateAnimation(float DeltaSeconds) override;

	protected:
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnStateFinished(EPigStates pigState);

	protected:
	UPROPERTY(BlueprintReadOnly)
	float m_fVelocity = 0.f;

	protected:
	UPROPERTY(BlueprintReadOnly)
	EPigStates m_xPigState = EPigStates::Default;

};
