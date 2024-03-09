#pragma once

#include "CoreMinimal.h"
#include "MovementControllerEvent.generated.h"

UENUM(BlueprintType)
enum class EMovementControllerEvent : uint8 {
	RotationFinished,
	Size
};