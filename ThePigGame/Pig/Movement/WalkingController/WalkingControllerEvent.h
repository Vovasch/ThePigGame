#pragma once

#include "CoreMinimal.h"
#include "WalkingControllerEvent.generated.h"

UENUM(BlueprintType)
enum class EWalkingControllerEvent : uint8 {
	WalkingCompleted,
	WalkingFailed,
	Size
};