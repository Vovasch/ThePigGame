#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Farm/Controllers/ConsumeSpotsController/ConsumeSpotsController.h"
#include "ConsumeSpotComponent.generated.h"

class AConsumeSource;
class ATrough;

UCLASS(BlueprintType, Blueprintable)
class THEPIGGAME_API UConsumeSpotComponent : public UStaticMeshComponent {
	GENERATED_BODY()

	public:
	bool IsAvailable() const;

	public:
	EConsumeSourceType GetSpotType() const;

	public:
	TOptional<float> TryConsumeOut(float amount, APig* consumer);

	public:
	AConsumeSource* GetConsumeSourceOwner() const;
};