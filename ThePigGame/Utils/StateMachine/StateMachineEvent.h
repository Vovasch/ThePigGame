#pragma once

#include "CoreMinimal.h"
#include "StateMachineEvent.generated.h"

UENUM(BlueprintType)
enum class EStateMachineEvent : uint8 {
	StateChanged,
	Size
};