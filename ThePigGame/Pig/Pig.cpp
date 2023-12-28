﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Pig.h"
#include "Components/TextRenderComponent.h"
#include "PigAI/PigAIController.h"
#include "PigAI/PigAIControllerEvent.h"
#include "../Farm/Farm.h"
#include "../Farm/EatingSpot.h"
#include "PigStateMachine/PigStateMachine.h"
#include "../Animation/PigAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../Levels/MainLevel/GameMainPlayerController.h"
#include "../Levels/MainLevel/MainScreenGameMode.h"
#include "Tasks/TaskDispatcher.h"
#include "Components/CapsuleComponent.h"
//#include "../Utils/StateMachine/StateEvent.h"

// Sets default values
APig::APig() {

	PigInfo = CreateDefaultSubobject<UTextRenderComponent, UTextRenderComponent>("PigInfo");
	PigInfo->SetupAttachment(GetRootComponent());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Animalia/Pig_F/Meshes/Pig_F.Pig_F'"));
	m_xAdultMesh = MeshAsset.Object.Get();

	ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/Pig/Animation/BigPig/ABP_BigPig.ABP_BigPig'"));
	m_xAdultAnimBlueprint = AnimObj.Object->GeneratedClass;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APig::BeginPlay() {
	Super::BeginPlay();
	
	Init();

	CreateStateMachine();
	CreateTaskDispatcher();

	GetPigAnimInstance()->InitAnimInstance(m_pStateMachine);
	m_pPigAIController->InitAIController(m_pStateMachine, m_pTaskDispatcher);

	SubscribeOnAnimations();
	SubscribeOnAIController();
}

// Called every frame
void APig::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	TickProviderTick(DeltaTime);

	m_pStateMachine->Tick(DeltaTime);
	m_pTaskDispatcher->Tick(DeltaTime);

	SetPigScale();
	SetMeshMorphs();
	CheckIfAdult();

	CheckBellyfulLevel();
	CheckEnergyLevel();

	if(m_xWeight.GetCurrent() <= m_xCriticalWeight.GetCurrent()) {
		// confisacate pig
	}

	if(m_xAge.GetCurrent() >= m_fMaxAgeAtSeconds) {
		// take pig out of bussiness into hospice
	}
	
	FillPigInfo();
}

void APig::Init() {
	auto gameMode = Cast<AMainScreenGameMode>(GetWorld()->GetAuthGameMode());

	m_fMaxAgeAtSeconds = m_xInitData.MaxAgeYears * gameMode->GetOneYearInSeconds();
	m_fMaxSizesAtSeconds = m_xInitData.MaxSizesAtAgeYears * gameMode->GetOneYearInSeconds();

	m_fWeightDeltaPerTick = (m_xInitData.CriticalWeightAtMaxAge - m_xInitData.CriticalWeightAtMinAge) / m_fMaxSizesAtSeconds;
	m_fWeightDeltaPerTick *= 1.5f;
	
	m_fAgeWhenAdultSeconds = m_xInitData.AgeOfAdultYears * gameMode->GetOneYearInSeconds();

	m_xAge.Init(this);
	m_xAge.GetMinMaxType().SetMinMax(0, m_fMaxAgeAtSeconds);


	m_xScale.Init(this);
	m_xScale.GetMinMaxType().SetMinMax(m_xInitData.ChildMinScale, m_xInitData.ChildMaxScale);
	m_xScale.CalcCoeff(m_xAge.GetCurrentPtr(), 0, m_fAgeWhenAdultSeconds);


	m_xBellyful.Init(this);
	m_xBellyful.GetMinMaxType().SetMinMax(m_xInitData.MinBellyful, m_xInitData.MaxBellyful);
	m_xBellyful.SetDelta(-m_fWeightDeltaPerTick);
	m_xBellyful.GetCurrentModifycationType().Set(41.f);


	m_xCriticalWeight.Init(this);
	m_xCriticalWeight.GetMinMaxType().SetMinMax(m_xInitData.CriticalWeightAtMinAge, m_xInitData.CriticalWeightAtMaxAge);
	m_xCriticalWeight.CalcCoeff(m_xAge.GetCurrentPtr(), 0, m_fMaxSizesAtSeconds);


	m_xMaxWeight.Init(this);
	m_xMaxWeight.GetMinMaxType().SetMinMax(m_xInitData.MaxWeightAtMinAge, m_xInitData.MaxWeightAtMaxAge);
	m_xMaxWeight.CalcCoeff(m_xAge.GetCurrentPtr(), 0, m_fMaxSizesAtSeconds);


	m_xWeight.Init(this);
	m_xWeight.GetMinMaxType().SetMinMax(m_xCriticalWeight.GetCurrentPtr(), m_xMaxWeight.GetCurrentPtr());
	m_xWeight.SetDelta(m_fWeightDeltaPerTick);
	m_xWeight.GetTickingType().SetCondition([this]() {
		return this->CanWeightTick();
		});
	m_xWeight.GetCurrentModifycationType().Set((m_xInitData.MaxWeightAtMinAge - m_xInitData.CriticalWeightAtMinAge) / 2 + m_xInitData.CriticalWeightAtMinAge);

	m_xEnergy.Init(this);
	m_xEnergy.GetMinMaxType().SetMinMax(m_xInitData.MinEnergy, m_xInitData.MaxEnergy);
	m_xEnergy.SetDelta(m_xInitData.LoosingEnergyDelta);
	m_xEnergy.GetCurrentModifycationType().Set(50);

	m_xMorph.Init(this);
	m_xMorph.GetMinMaxType().SetMinMax(m_xInitData.MorphTargetMinValue, m_xInitData.MorphTargetMaxValue);
	m_xMorph.CalcCoeff(m_xWeight.GetCurrentPtr(), m_xCriticalWeight.GetCurrentPtr(), m_xMaxWeight.GetCurrentPtr());
}

void APig::SetPigAIController(APigAIController* AIContoller) {
	m_pPigAIController = AIContoller;
}

void APig::SetPigScale() {
	auto currScale = m_xScale.GetCurrent();
	GetCapsuleComponent()->SetWorldScale3D({ currScale, currScale, currScale });
	//GetMesh()->SetWorldScale3D({ currScale, currScale, currScale });
}

void APig::SetMeshMorphs() {
	GetMesh()->SetMorphTarget(TEXT("Pig_Big"), m_xMorph.GetCurrent());
}

void APig::CheckBellyfulLevel() {
	auto bellyfulLevel = m_xBellyful.GetCurrent();

	if(bellyfulLevel <= m_xInitData.BellyfulLevelToWantToEat && m_pStateMachine->GetCurrentStateType()!=EPigStates::Eating /*also check if not waiting for eating spot*/) {
		AddTask(ETaskType::GoToEat);
	} else if(bellyfulLevel >= m_xInitData.BellyfulLevelToStopEating) {
		m_pStateMachine->TryChangeState(EPigStates::Default);
	}
}

void APig::CheckEnergyLevel() {
	auto energyLevel = m_xEnergy.GetCurrent();
	
	if(energyLevel <= m_xInitData.EnergyLevelToWantToSleepLevel) {
		//AddTask(ETaskType::GoToSleep);
	} else if(energyLevel >= m_xInitData.EnergyLevelSleepedEnought) {
		m_pStateMachine->TryChangeState(EPigStates::StandingUp);
	}
}

APigAIController* APig::GetPigAIController() {
	return m_pPigAIController;
}


AFarm* APig::GetOwnerFarm() {
	return Cast<AFarm>(GetOwner());
}


bool APig::CanWeightTick() {
	auto currentBellyful = m_xBellyful.GetCurrent();
	if(currentBellyful >= m_xInitData.BellyfulLevelToGetFat) {
		m_xWeight.SetDelta(m_fWeightDeltaPerTick);
		return true;
	} else if(currentBellyful <= m_xInitData.BellyfulLevelToGetThin) {
		m_xWeight.SetDelta(-m_fWeightDeltaPerTick);
		return true;
	}
	return false;
}

void APig::FillPigInfo() {
	FString str(UTF8_TO_TCHAR("Current age = "));

	auto currentAge = m_xAge.GetCurrent();
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

	PigInfo->SetText(FText::FromString(str));

	auto plc = Cast<AGameMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	plc->SetPigInfo(str);
	

}

UPigStateMachine* APig::GetPigStateMachine() {
	return m_pStateMachine;
}

void APig::CreateStateMachine() {
	m_pStateMachine = NewObject<UPigStateMachine>();
	m_pStateMachine->SetPigOwner(this);
	SubscribeOnStatesEvents();
}

void APig::SubscribeOnStatesEvents() {
	SubscribeOnEatingState();
	SubscribeOnSleepingState();
}

void APig::SubscribeOnSleepingState() {
	auto pigSleepingState = m_pStateMachine->GetState(EPigStates::Sleeping);

	pigSleepingState->Subscribe(EStateEvent::Start, [this]() {
		m_xEnergy.SetDelta(m_xInitData.GainignEnergyDelta);
	});

	pigSleepingState->Subscribe(EStateEvent::End, [this]() {
		m_xEnergy.SetDelta(m_xInitData.LoosingEnergyDelta);
	});
}


void APig::SubscribeOnEatingState() {
	auto pigEatState = m_pStateMachine->GetState(EPigStates::Eating);

	pigEatState->Subscribe(EStateEvent::Start, [this]() {
		m_xBellyful.SetDelta(2);
		m_pPigAIController->OnStartedEating();
	});

	pigEatState->Subscribe(EStateEvent::End, [this]() {
		m_xBellyful.SetDelta(-m_fWeightDeltaPerTick);
		m_pPigAIController->OnFinishedEating();
	});
}

UPigAnimInstance* APig::GetPigAnimInstance() {
	return Cast<UPigAnimInstance>(GetMesh()->GetAnimInstance());
}

void APig::SubscribeOnAnimations() {
	auto animInstace = GetPigAnimInstance();
	animInstace->Subscribe(EPigStates::LayingDown, [this]() {
		m_pStateMachine->TryChangeState(EPigStates::Sleeping);
	});

	animInstace->Subscribe(EPigStates::StandingUp, [this]() {
		m_pStateMachine->TryChangeState(EPigStates::Default);
	});
}

UTaskDispatcher* APig::GetTaskDispatcher() {
	return m_pTaskDispatcher;
}

void APig::CreateTaskDispatcher() {
	m_pTaskDispatcher = NewObject<UTaskDispatcher>();
	m_pTaskDispatcher->Init(this);
}

void APig::AddTask(ETaskType taskType) {
	m_pTaskDispatcher->AddTask(taskType);
}

void APig::SubscribeOnAIController() {
	m_pPigAIController->Subscribe(EPigAIControllerEvent::CanStartEating, [this]() {
			m_pStateMachine->TryChangeState(EPigStates::Eating);
	});

	m_pPigAIController->Subscribe(EPigAIControllerEvent::ReachedSleepingSpot, [this]() {
		m_pStateMachine->TryChangeState(EPigStates::LayingDown);
	});
}


void APig::CheckIfAdult() {
	if(!m_bIsAdult && m_xAge.GetCurrent() >= m_fAgeWhenAdultSeconds) {
		m_bIsAdult = true;

		m_xScale.GetMinMaxType().SetMinMax(m_xInitData.AdultMinScale, m_xInitData.AdultMaxScale);
		m_xScale.CalcCoeff(m_xAge.GetCurrentPtr(), m_fAgeWhenAdultSeconds, m_fMaxSizesAtSeconds);

		GetPigAnimInstance()->DisengageAnimInstance(m_pStateMachine);

		GetMesh()->SetSkeletalMesh(m_xAdultMesh);
		GetMesh()->SetAnimInstanceClass(m_xAdultAnimBlueprint);

		GetPigAnimInstance()->InitAnimInstance(m_pStateMachine);
	}
}


