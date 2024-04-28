#pragma once

#include "CoreMinimal.h"
#include "RotationControllerEvent.generated.h"

UENUM(BlueprintType)
enum class ERotationControllerEvent : uint8 {
	RotationCompleted,
	Size
};