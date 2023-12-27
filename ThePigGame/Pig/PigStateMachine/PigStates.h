#pragma once

#include "CoreMinimal.h"
#include "PigStates.generated.h"

UENUM(BlueprintType)
enum class EPigStates : uint8 {
	Default,
	Eating,	
	Sleeping,
	LayingDown,
	StandingUp,
	Size
};