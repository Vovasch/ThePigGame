// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../../UI/GameScreen.h"

void AGameMainPlayerController::BeginPlay() {
	Super::BeginPlay();

	if(!MainScreen) return;

	m_pGameScreen = CreateWidget<UGameScreen>(this, MainScreen);

	if(!m_pGameScreen) return;

	m_pGameScreen->AddToViewport();

}


void AGameMainPlayerController::SetPigInfo(const FString& info) {
	m_pGameScreen->SetPigsInfo(info);
}