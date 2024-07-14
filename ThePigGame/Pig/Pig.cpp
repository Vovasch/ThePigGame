// Fill out your copyright notice in the Description page of Project Settings.


#include "Pig.h"
#include "Components/TextRenderComponent.h"
#include "PigAI/PigAIController.h"
#include "../Farm/Farm.h"
#include "PigStateMachine/PigStateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "../Levels/MainLevel/GameMainPlayerController.h"
#include "AnimInstance/AnimInstanceController.h"
#include "AnimInstance/PigAnimInstance.h"
#include "Tasks/Base/TaskDispatcher.h"
#include "Movement/MovementController.h"
#include "PropertyControllers/SupremePropertyController/SupremePropertyController.h"

APig::APig() {

	PrimaryActorTick.bCanEverTick = true;

	PigInfo = CreateDefaultSubobject<UTextRenderComponent, UTextRenderComponent>("PigInfo");
	PigInfo->SetupAttachment(GetRootComponent());
	PigInfo->bHiddenInGame = true;
}

void APig::BeginPlay() {
	Super::BeginPlay();

	CreateObjects();

	InitObjects();

	AfterInitObjects();
}

void APig::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_pPropertyController->Tick(DeltaTime);
	m_pStateMachine->Tick(DeltaTime);
	m_pTaskDispatcher->Tick(DeltaTime);
	m_pMovementController->Tick(DeltaTime);
	
	FillPigInfo();
}

void APig::CreateObjects() {
	m_pPropertyController = NewObject<USupremePropertyController>(this);
	m_pStateMachine = NewObject<UPigStateMachine>(this);
	m_pTaskDispatcher = NewObject<UTaskDispatcher>(this);
	m_pMovementController = NewObject<UMovementController>(this);
	m_pAnimInstanceController = NewObject<UAnimInstanceController>(this);
}

void APig::InitObjects() {
	m_pPropertyController->Init(this);
	m_pStateMachine->Init(this);
	m_pTaskDispatcher->Init(this);
	m_pMovementController->Init(this);
	m_pAnimInstanceController->Init(this);

	GetPigAnimInstance()->Init(this);
}

void APig::AfterInitObjects() {
	m_pStateMachine->AfterInit();
}

AFarm* APig::GetOwnerFarm() {
	return Cast<AFarm>(GetOwner());
}

void APig::FillPigInfo() {
	FString str(UTF8_TO_TCHAR("Current age = "));

	/*auto currentAge = m_xAge.GetCurrent();
	auto gameMode = Cast<AMainScreenGameMode>(GetWorld()->GetAuthGameMode());
	str += FString::SanitizeFloat(m_xAge.GetCurrent() / gameMode->GetOneYearInSeconds());
	str += " years";
	str += "\n";

	str += "Current weight = ";
	str += FString::SanitizeFloat(m_xWeight.GetCurrent());
	str += " kg ";

	{
		auto weight = 100.f * (m_xWeight.GetCurrent() - m_xCriticalWeight.GetCurrent()) / (m_xMaxWeight.GetCurrent() - m_xCriticalWeight.GetCurrent());
		str += FString::SanitizeFloat(weight);
	}
	str += " %";
	str += "\n";


	str += "Current bellyful = ";
	str += FString::SanitizeFloat(m_xBellyful.GetCurrent());
	str += "\n";

	str += "Current energy = ";
	str += FString::SanitizeFloat(m_xEnergy.GetCurrent());

	PigInfo->SetText(FText::FromString(str));*/

	auto plc = Cast<AGameMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	plc->SetPigInfo(str);
}

const UPigInitData* APig::GetInitData() {
	return m_pInitData;
}

USupremePropertyController* APig::GetPropertyController() {
	return m_pPropertyController;
}

UPigStateMachine* APig::GetPigStateMachine() {
	return m_pStateMachine;
}

UTaskDispatcher* APig::GetTaskDispatcher() {
	return m_pTaskDispatcher;
}

UMovementController* APig::GetMovementController() {
	return m_pMovementController;
}

APigAIController* APig::GetPigAIController() {
	//TODO: check if this really works
	return Cast<APigAIController>(Controller);
}

UAnimInstanceController* APig::GetAnimInstanceController() {
	return m_pAnimInstanceController;
}

UPigAnimInstance* APig::GetPigAnimInstance() {
	return Cast<UPigAnimInstance>(GetMesh()->GetAnimInstance());
}

