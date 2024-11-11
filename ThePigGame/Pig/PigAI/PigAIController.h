// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PigAIController.generated.h"

class AWalkingController;
class APig;
class UTaskDispatcher;

/**
 * 
 */
UCLASS()
class THEPIGGAME_API APigAIController : public AAIController {
	GENERATED_BODY()

	public:
	APigAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
