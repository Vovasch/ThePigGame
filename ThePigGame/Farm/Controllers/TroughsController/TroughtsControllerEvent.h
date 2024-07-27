#pragma once

#include "CoreMinimal.h"
#include "TroughtsControllerEvent.generated.h"

UENUM()
enum class ETroughsControllerEvent : uint8 {
	EatingSpotFreed, // fires if all eating spots were occupied and then one become available
	Size
};
