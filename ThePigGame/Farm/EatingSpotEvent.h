#pragma once

#include "CoreMinimal.h"
#include "EatingSpotEvent.generated.h"

UENUM(BlueprintType)
enum class EEatingSpotEvent : uint8 {
	Occupied,
	Freed,
	Size
};