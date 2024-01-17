// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScreenGameMode.h"

AMainScreenGameMode::AMainScreenGameMode() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMainScreenGameMode::BeginPlay() {
	auto half = m_fDayLengthInSeconds / 2;
	auto quarter = half / 2;

	// this will make day to start when sun is just under the landscape before the sunrise
	// and night to start when sun is has just got under the landscape after the sunset
	auto num = m_fDayLengthInSeconds / 36; 

	m_fStartOfMorningSeconds = quarter - num;
	m_fStartOfNightSeconds = half + quarter + num;

}

void AMainScreenGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	m_fCurrentDayProgress += DeltaSeconds;

	if(m_fCurrentDayProgress >= m_fDayLengthInSeconds) {
		m_fCurrentDayProgress = 0;
		++m_uDayCounter;
		m_bMorningEventFired = false;
		m_bNightEventFired = false;
	}

	if(!m_bMorningEventFired && m_fCurrentDayProgress >= m_fStartOfMorningSeconds) {
		m_bMorningEventFired = true;
		OnEvent(EGameModeEvent::MorningStarted);
	}

	if(!m_bNightEventFired && m_fCurrentDayProgress >= m_fStartOfNightSeconds) {
		m_bNightEventFired = true;
		OnEvent(EGameModeEvent::NightStarted);
	}

}

const float *const AMainScreenGameMode::GetDayProgressPtr() {
	return &m_fCurrentDayProgress;
}

float AMainScreenGameMode::GetDayLengthInSeconds() {
	return m_fDayLengthInSeconds;
}

float AMainScreenGameMode::GetOneYearInSeconds() {
	return m_fOneYearInSeconds;
}