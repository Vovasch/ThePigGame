// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenLevelBlueprint.h"
#include "Levels/MainLevel/MainScreenGameMode.h"

void AMainScreenLevelBlueprint::BeginPlay() {
	Super::BeginPlay();

	auto gameMode = Cast<AMainScreenGameMode>(GetWorld()->GetAuthGameMode());

	m_xLightRotation.Init(this);
	m_xLightRotation.GetMinMaxType().SetMinMax(m_fLightMinRotation, m_fLightMaxRotation);
	m_xLightRotation.CalcCoeff(gameMode->GetDayProgressPtr(), 0.f, gameMode->GetDayLengthInSeconds());
	m_xLightRotation.GetCurrentModifycationType().Set(m_fLightMinRotation);

	gameMode->Subscribe(EGameModeEvent::MorningStarted, [this]() {
		this->OnMorningStarted();
	});

	gameMode->Subscribe(EGameModeEvent::NightStarted, [this]() {
		this->OnNightStarted();
	});
	
}


void AMainScreenLevelBlueprint::Tick(float delta) {
	Super::Tick(delta);
	IPropertyTickProvider::TickProviderTick(delta);
}


float AMainScreenLevelBlueprint::GetCurrentLightRotation() {
	return m_xLightRotation.GetCurrent();
}