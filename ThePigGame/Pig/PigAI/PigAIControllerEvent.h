#pragma once

#include "CoreMinimal.h"
#include "PigAIControllerEvent.generated.h"

UENUM(BlueprintType)
enum class EPigAIControllerEvent: uint8 {
	ReachedEatingSpot,
	CanStartEating,
	UableToStartEating,
	ReachedSleepingSpot,
	Size
};