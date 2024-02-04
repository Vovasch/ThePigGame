#pragma once

#include "CoreMinimal.h"
#include "FarmEvent.generated.h"

UENUM(BlueprintType)
enum class EFarmEvent : uint8 {
	EatingSpotFreed, // fires if all eating spots were occupied and than one become available
	Size
};