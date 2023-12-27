#pragma once

#include "CoreMinimal.h"
#include "StateEvent.generated.h"

UENUM(BlueprintType)
enum class EStateEvent : uint8 {
	Start,
	End,
	Size
};