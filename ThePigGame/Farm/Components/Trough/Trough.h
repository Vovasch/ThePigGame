// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSource.h"
#include "Trough.generated.h"

class UTextRenderComponent;
class UStaticMeshComponent;

UCLASS()
class THEPIGGAME_API ATrough : public AConsumeSource {
	GENERATED_BODY()

	ATrough();
};
