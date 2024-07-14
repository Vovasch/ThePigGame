#pragma once

#include "CoreMinimal.h"
#include "PigAIControllerEvent.generated.h"

UENUM(BlueprintType)
enum class EPigAIControllerEvent: uint8 {
	// todo remove all this shit, make only reached target loc and failed to reach target loc
	// todo and move into the movement controller
	ReachedEatingSpot,
	FailedToReachEatingSpot,

	ReachedSleepingSpot,
	FailedToReachSleepingSpot,

	ReachedRandomLocation,
	FailedToReachRandomLocation,

	Size
};