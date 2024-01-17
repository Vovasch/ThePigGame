#pragma once

#include "CoreMinimal.h"
#include "GameModeEvent.generated.h"

UENUM(BlueprintType)
enum class EGameModeEvent : uint8 {
	MorningStarted,
	NightStarted,
	Size
};