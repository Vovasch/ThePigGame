// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeEvent.h"
#include "../../Utils/EventHandler/TEventHandler.h"
#include "MainScreenGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THEPIGGAME_API AMainScreenGameMode : public AGameModeBase, public TEventHandler<EGameModeEvent> {
	GENERATED_BODY()
	
	public:
	AMainScreenGameMode();

	public:
	virtual void BeginPlay() override;

	public:
	float GetOneYearInSeconds();

	public:
	virtual void Tick(float DeltaSeconds) override;

	public:
	const float *const GetDayProgressPtr();
	float GetDayLengthInSeconds();

	protected:
	UPROPERTY(EditAnywhere)
	float m_fOneYearInSeconds = 10.f;

	protected:
	UPROPERTY(EditAnywhere)
	float m_fDayLengthInSeconds = 20.f;

	protected:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "100"))
	float m_fDayProgressOnBeginPlayInPercentages = 0.f;

	protected:
	UPROPERTY(VisibleAnywhere)
	float m_fCurrentDayProgress = 0.f;

	protected:
	UPROPERTY(VisibleAnywhere)
	uint32 m_uDayCounter = 0.f;

	protected:
	float m_fStartOfMorningSeconds = 0.f;
	float m_fStartOfNightSeconds = 0.f;

	protected:
	bool m_bMorningEventFired = false;
	bool m_bNightEventFired = false;

};

