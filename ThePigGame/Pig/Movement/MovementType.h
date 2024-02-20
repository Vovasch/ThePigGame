#pragma once

#include "CoreMinimal.h"
#include "MovementType.generated.h"

UENUM(BlueprintType)
enum class EMovementType : uint8 {
	Sneak,
	SlowWalk,
	Walk,
	Trot,
	Default = Trot,
	Run,
	Sprint,
	Size
};