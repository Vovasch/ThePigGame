#pragma once

#include "CoreMinimal.h"
#include "TargetLocationTypes.generated.h"

UENUM(BlueprintType)
enum class ETargetLocationTypes : uint8 {
	None,
	EatingSpot,
	SleepingSpot,
	RandomLocation
};