// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainScreenGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THEPIGGAME_API AMainScreenGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
	float GetOneYearInSeconds();


	protected:
	UPROPERTY(EditAnywhere)
	float m_fOneYearInSeconds = 10.f;

};
