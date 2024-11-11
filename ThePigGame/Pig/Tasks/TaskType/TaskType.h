#pragma once

#include "CoreMinimal.h"
#include "TaskType.generated.h"

UENUM(BlueprintType)
enum class ETaskType : uint8 {
	None,
	GoToConsumeSpot,
	GoToSleep,
	GoToRandomLocation,
	Size
};