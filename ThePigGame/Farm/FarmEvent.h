#pragma once

#include "CoreMinimal.h"
#include "FarmEvent.generated.h"

UENUM(BlueprintType)
enum class EFarmEvent : uint8 {
	EatingSpotFreed,
	Size
};