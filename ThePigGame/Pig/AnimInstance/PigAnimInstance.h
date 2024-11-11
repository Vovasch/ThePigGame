// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "ThePigGame/Pig/PigStateMachine/PigStates.h"
#include "ThePigGame/Utils/EventHandler/TEventHandler.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "PigAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEPIGGAME_API UPigAnimInstance : public UAnimInstance, public TEventHandler<EPigStates>, public ICachedPigDataUser {
	GENERATED_BODY()
	
	public:
	virtual void Init(APig* pig) override;

	protected:
	void SetPigState(EPigStates pigState);

	protected:
	virtual void PreUpdateAnimation(float DeltaSeconds) override;

	protected:
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnStateFinished(EPigStates pigState);

	protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (BlueprintThreadSafe))
	EConsumeSourceType GetCurrentConsumeSpotType();

	protected:
	UPROPERTY(BlueprintReadOnly)
	float m_fVelocity = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float m_fRotation = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	EPigStates m_xPigState = EPigStates::Default;

};
