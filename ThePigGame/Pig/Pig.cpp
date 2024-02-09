// Fill out your copyright notice in the Description page of Project Settings.


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
#include "Tasks/Base/TaskDispatcher.h"
#include "Components/CapsuleComponent.h"

APig::APig() {

	PigInfo = CreateDefaultSubobject<UTextRenderComponent, UTextRenderComponent>("PigInfo");
	PigInfo->SetupAttachment(GetRootComponent());
	PigInfo->bHiddenInGame = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Animalia/Pig_F/Meshes/Pig_F.Pig_F'"));
	m_xAdultMesh = MeshAsset.Object.Get();

	ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/Pig/Animation/BigPig/ABP_BigPig.ABP_BigPig'"));
	m_xAdultAnimBlueprint = AnimObj.Object->GeneratedClass;

	PrimaryActorTick.bCanEverTick = true;

}

void APig::BeginPlay() {
	Super::BeginPlay();
	
	InitProperties();

	CreateObjects();

	InitObjects();

	AfterInitObjects();
}

void APig::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	TickProviderTick(DeltaTime);

	m_pStateMachine->Tick(DeltaTime);
	m_pTaskDispatcher->Tick(DeltaTime);

	CheckIfAdult();
	SetPigScale();
	SetMeshMorphs();

	CheckBellyfulLevel();
	CheckEnergyLevel();

	if(m_xWeight.GetCurrent() <= m_xCriticalWeight.GetCurrent()) {
		// confisacate pig
		//OnEvent(EPigEvent::RemovedFromFarm);
	}

	if(m_xAge.GetCurrent() >= m_fMaxAgeAtSeconds) {
		// take pig out of bussiness into hospice
		//OnEvent(EPigEvent::RemovedFromFarm);
	}
	
	FillPigInfo();
}

void APig::InitProperties() {
	auto gameMode = Cast<AMainScreenGameMode>(GetWorld()->GetAuthGameMode());

	m_fMaxAgeAtSeconds = m_xInitData.MaxAgeYears * gameMode->GetOneYearInSeconds();
	m_fMaxSizesAtSeconds = m_xInitData.MaxSizesAtAgeYears * gameMode->GetOneYearInSeconds();

	m_fWeightDeltaPerTick = (m_xInitData.CriticalWeightAtMaxAge - m_xInitData.CriticalWeightAtMinAge) / m_fMaxSizesAtSeconds;
	m_fWeightDeltaPerTick *= 1.5f;
	
	m_fWeightDeltaPerTick = 0.5;

	m_fAgeWhenAdultSeconds = m_xInitData.AgeOfAdultYears * gameMode->GetOneYearInSeconds();

	m_xAge.Init(this);
	m_xAge.GetMinMaxType().SetMinMax(0, m_fMaxAgeAtSeconds);


	m_xScale.Init(this);
	m_xScale.GetMinMaxType().SetMinMax(m_xInitData.ChildMinScale, m_xInitData.ChildMaxScale);
	m_xScale.CalcCoeff(m_xAge.GetCurrentPtr(), 0, m_fAgeWhenAdultSeconds);


	m_xBellyful.Init(this);
	m_xBellyful.GetMinMaxType().SetMinMax(m_xInitData.MinBellyful, m_xInitData.MaxBellyful);
	m_xBellyful.SetDelta(-m_fWeightDeltaPerTick);
	m_xBellyful.GetCurrentModifycationType().Set(100.f);


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

void APig::CreateObjects() {
	m_pStateMachine = NewObject<UPigStateMachine>();
	m_pTaskDispatcher = NewObject<UTaskDispatcher>();
}
void APig::InitObjects() {
	m_pStateMachine->Init(this);
	m_pTaskDispatcher->Init(this);

	GetPigAnimInstance()->Init(this);
	GetPigAIController()->Init();
}
void APig::AfterInitObjects() {
	m_pStateMachine->AfterInit();
	SubscribeOnAnimInstance();
}

void APig::SetPigAIController(APigAIController* AIContoller) {
	m_pPigAIController = AIContoller;
}

void APig::SetPigScale() {
	auto currScale = m_xScale.GetCurrent();
	//GetCapsuleComponent()->SetWorldScale3D({ currScale, currScale, currScale });
	GetMesh()->SetWorldScale3D({ currScale, currScale, currScale });
}

void APig::SetMeshMorphs() {
	GetMesh()->SetMorphTarget(TEXT("Pig_Big"), m_xMorph.GetCurrent());
}

void APig::CheckBellyfulLevel() {
	if(m_xBellyful.GetCurrent() <= m_xInitData.BellyfulLevelToWantToEat && m_pStateMachine->GetCurrentStateType() != EPigStates::Eating && !m_bIsWaitingForEatingSpot) {
		AddTask(ETaskType::GoToEat);
	}
}

void APig::CheckEnergyLevel() {
	auto energyLevel = m_xEnergy.GetCurrent();
	
	if(energyLevel <= m_xInitData.EnergyLevelToWantToSleepLevel) {
		// TODO: integrate into sleeping system
		//AddTask(ETaskType::GoToSleep);
	} else if(energyLevel >= m_xInitData.EnergyLevelSleepedEnought) {
		// TODO: integrate into sleeping system
		//m_pStateMachine->TryChangeState(EPigStates::StandingUp);
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

UPigAnimInstance* APig::GetPigAnimInstance() {
	return Cast<UPigAnimInstance>(GetMesh()->GetAnimInstance());
}

void APig::SubscribeOnAnimInstance() {
	auto animInstace = GetPigAnimInstance();
	animInstace->Subscribe(this, EPigStates::LayingDown, [this]() {
		m_pStateMachine->TryChangeState(EPigStates::Sleeping);
	});

	animInstace->Subscribe(this, EPigStates::StandingUp, [this]() {
		m_pStateMachine->TryChangeState(EPigStates::Default);
	});
}

void APig::UnsubscribeFromAnimInstance() {
	GetPigAnimInstance()->Unsibscribe(this);
}

UTaskDispatcher* APig::GetTaskDispatcher() {
	return m_pTaskDispatcher;
}

void APig::AddTask(ETaskType taskType) {
	m_pTaskDispatcher->AddTask(taskType);
}

void APig::CheckIfAdult() {
	if(!m_bIsAdult && m_xAge.GetCurrent() >= m_fAgeWhenAdultSeconds) {
		m_bIsAdult = true;

		m_xScale.GetMinMaxType().SetMinMax(m_xInitData.AdultMinScale, m_xInitData.AdultMaxScale);
		m_xScale.CalcCoeff(m_xAge.GetCurrentPtr(), m_fAgeWhenAdultSeconds, m_fMaxSizesAtSeconds);

		GetPigAnimInstance()->DisengageAnimInstance();
		UnsubscribeFromAnimInstance();

		GetMesh()->SetSkeletalMesh(m_xAdultMesh, false);
		GetMesh()->SetAnimInstanceClass(m_xAdultAnimBlueprint);

		GetPigAnimInstance()->Init(this);
		SubscribeOnAnimInstance();
	}
}

void APig::SetWaitingForEatingSpot(bool isWaiting) {
	m_bIsWaitingForEatingSpot = isWaiting;
}

void APig::GoToSleep() {
	m_xEnergy.SetDelta(m_xInitData.GainignEnergyDelta);
	AddTask(ETaskType::GoToSleep);
}

void APig::WakeUp() {
	m_xEnergy.SetDelta(m_xInitData.LoosingEnergyDelta);
	m_pStateMachine->TryChangeState(EPigStates::StandingUp);
}

void APig::StartEating() {
	m_pPigAIController->OnStartedEating();
	m_pStateMachine->TryChangeState(EPigStates::Eating);
	m_xBellyful.SetDelta(2);
}

void APig::StopEating() {
	m_pPigAIController->OnFinishedEating();
	m_pStateMachine->TryChangeState(EPigStates::Default);
	m_xBellyful.SetDelta(-m_fWeightDeltaPerTick);
}

float APig::GetCurrentBellyful() {
	return m_xBellyful.GetCurrent();
}

float APig::GetBellyfulLevelToStopEating() {
	return m_xInitData.BellyfulLevelToStopEating;
}