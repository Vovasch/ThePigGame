// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScreen.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void UGameScreen::NativeConstruct() {
	Super::NativeConstruct();

	auto rootWidget = Cast<UPanelWidget>(GetRootWidget());

	m_pPigsInfo = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PigsInfo"));
	rootWidget->AddChild(m_pPigsInfo);
}

void UGameScreen::SetPigsInfo(const FString& info) {
	m_pPigsInfo->SetText(FText::FromString(info));
}