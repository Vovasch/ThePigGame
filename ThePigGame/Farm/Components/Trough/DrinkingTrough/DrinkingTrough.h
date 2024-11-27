#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSource.h"
#include "DrinkingTrough.generated.h"

class UTextRenderComponent;
class UStaticMeshComponent;

UCLASS()
class THEPIGGAME_API ADrinkingTrough : public AConsumeSource {
	GENERATED_BODY()

	public:
	EConsumeSourceType GetConsumeType() override;
};