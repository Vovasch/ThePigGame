// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameMainPlayerController.generated.h"

class UGameScreen;

/**
 * 
 */
UCLASS()
class THEPIGGAME_API AGameMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;

	public:
	void SetPigInfo(const FString& info);

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UGameScreen> MainScreen;

	protected:
	UGameScreen* m_pGameScreen = nullptr;

};
