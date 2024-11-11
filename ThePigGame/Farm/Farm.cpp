#include "Farm.h"
#include "Components/ConsumeSource/ConsumeSource.h"
#include "Components/ConsumeSpotComponent/ConsumeSpotComponent.h"
#include "Controllers/PigsController/PigsController.h"
#include "Controllers/SleepingPigsController/SleepingPigsController.h"
#include "Engine/World.h"

AFarm::AFarm() {
	PrimaryActorTick.bCanEverTick = true;

	// todo make functions Create Components and Create Controllers
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));

	m_pSleepingArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SleepringArea"));
	m_pSleepingArea->SetupAttachment(RootComponent);

	m_pPigsController = CreateDefaultSubobject<UPigsController>(TEXT("PigsController"));
	m_pSleepingPigsController = CreateDefaultSubobject<USleepingPigsController>(TEXT("SleepringPigsController"));
	m_pConsumeSpotsController = CreateDefaultSubobject<UConsumeSpotsController>(TEXT("ConsumeSpotsController"));
}

void AFarm::BeginPlay() {
	Super::BeginPlay();

	InitControllers();

	m_pPigsController->BeginPlay();
}

void AFarm::InitControllers() {
	m_pPigsController->Init();
	m_pSleepingPigsController->Init();
	m_pConsumeSpotsController->Init();
}

UPigsController* AFarm::GetPigsController() {
	return m_pPigsController;
}

USleepingPigsController* AFarm::GetSleepingPigsController() {
	return m_pSleepingPigsController;
}

UConsumeSpotsController* AFarm::GetConsumeSpotsController() {
	return m_pConsumeSpotsController;
}

UStaticMeshComponent* AFarm::GetSleepingArea() {
	return m_pSleepingArea;
}
