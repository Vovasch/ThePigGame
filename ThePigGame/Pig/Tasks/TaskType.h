#pragma once

#include "CoreMinimal.h"
#include "TaskType.generated.h"

UENUM(BlueprintType)
enum class ETaskType : uint8 {
	None,
	GoToEat,
	GoToSleep,
	GoToRandomLocation,
	Size
};