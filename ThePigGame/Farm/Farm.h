// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Farm.generated.h"

class UPigsController;
class USleepingPigsController;
class UTroughsController;

UCLASS()
class THEPIGGAME_API AFarm : public AActor {
	GENERATED_BODY()

	public:	
	AFarm();

	protected:
	virtual void BeginPlay() override;

	protected:
	void CreateControllers();
	void InitControllers();

	public:
	UPigsController* GetPigsController();
	USleepingPigsController* GetSleepingPigsController();
	UTroughsController* GetTroughsController();

	public:
	UStaticMeshComponent* GetSleepingArea();

	protected:
	UPROPERTY()
	UPigsController* m_pPigsController = nullptr;

	UPROPERTY()
	USleepingPigsController* m_pSleepingPigsController = nullptr;

	UPROPERTY()
	UTroughsController* m_pTroughController = nullptr;

	protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_pSleepingArea = nullptr;
};
