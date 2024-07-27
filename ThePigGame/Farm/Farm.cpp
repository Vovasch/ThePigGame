// Fill out your copyright notice in the Description page of Project Settings.

#include "Farm.h"
#include "Controllers/PigsController/PigsController.h"
#include "Controllers/SleepingPigsController/SleepingPigsController.h"
#include "Engine/World.h"
#include "Controllers/TroughsController/TroughsController.h"

AFarm::AFarm() {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));

	m_pSleepingArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SleepringArea"));
	m_pSleepingArea->SetupAttachment(RootComponent);
}

void AFarm::BeginPlay() {
	Super::BeginPlay();

	CreateControllers();
	InitControllers();

	m_pPigsController->BeginPlay();
}

void AFarm::CreateControllers() {
	m_pPigsController = NewObject<UPigsController>(this);
	m_pSleepingPigsController = NewObject<USleepingPigsController>(this);
	m_pTroughController = NewObject<UTroughsController>(this);
}

void AFarm::InitControllers() {
	m_pPigsController->Init();
	m_pSleepingPigsController->Init();
	m_pTroughController->Init();
}


UPigsController* AFarm::GetPigsController() {
	return m_pPigsController;
}

USleepingPigsController* AFarm::GetSleepingPigsController() {
	return m_pSleepingPigsController;
}

UTroughsController* AFarm::GetTroughsController() {
	return m_pTroughController;
}

UStaticMeshComponent* AFarm::GetSleepingArea() {
	return m_pSleepingArea;
}
