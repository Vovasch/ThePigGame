#pragma once

#include "CoreMinimal.h"
#include "PigEvent.generated.h"

UENUM(BlueprintType)
enum class EPigEvent : uint8 {
	RemovedFromFarm,
	Size
};