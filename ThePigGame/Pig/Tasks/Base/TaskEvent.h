#pragma once

#include "CoreMinimal.h"
#include "TaskEvent.generated.h"

UENUM(BlueprintType)
enum class ETaskEvent : uint8 {
	Start,
	End,
	Success,
	Fail,
	Interrupt,
	Size
};

