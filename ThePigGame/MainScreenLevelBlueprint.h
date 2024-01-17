// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Utils/Property/Property.h"
#include "MainScreenLevelBlueprint.generated.h"

/**
 * 
 */
UCLASS()
class THEPIGGAME_API AMainScreenLevelBlueprint : public ALevelScriptActor, public IPropertyTickProvider {
	GENERATED_BODY()
	
	protected:
	virtual void BeginPlay() override;

	protected:
	virtual void Tick(float delta) override;

	protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnMorningStarted();

	protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnNightStarted();

	public:
	UFUNCTION(BlueprintCallable)
	float GetCurrentLightRotation();

	protected:
	UPROPERTY(EditAnywhere)
	float m_fLightMaxRotation = 90.f;

	protected:
	UPROPERTY(EditAnywhere)
	float m_fLightMinRotation = -270.f;

	protected:
	TCoefficientalProperty<TMinMaxStatic, TInstantTicking, TAllowedCurrentModification> m_xLightRotation;
};
