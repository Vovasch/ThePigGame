// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utils/EventHandler/TEventHandler.h"
#include "FarmEvent.h"
#include "Farm.generated.h"

class ATrough;
class UEatingSpot;
class USleepingArea;
class APig;

UCLASS()
class THEPIGGAME_API AFarm : public AActor, public TEventHandler<EFarmEvent> {
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	AFarm();

	public:
	UEatingSpot* GetAvailableEatingSpot();

	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	public:
	const USleepingArea* GetSleepingArea();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	protected:
	void BindOnPig(APig* pig);

	protected:
	UPROPERTY()
	TArray<class APig*> m_vPigs;

	protected:
	TArray<ATrough*> m_vTroughs;

	protected:
	USleepingArea* m_pSleepingArea = nullptr;

	protected:
	int32 m_iTotalEatingSpots = 0;
	int32 m_iAvailableEatingSpots = 0;

};
