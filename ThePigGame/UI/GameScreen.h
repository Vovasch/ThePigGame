// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScreen.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class THEPIGGAME_API UGameScreen : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	virtual void NativeConstruct() override;

	public:
	void SetPigsInfo(const FString& info);

	protected:
	UPROPERTY(EditAnyWhere);
	UTextBlock* m_pPigsInfo = nullptr;

};
