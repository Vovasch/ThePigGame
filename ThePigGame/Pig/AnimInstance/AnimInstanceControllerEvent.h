#pragma once

#include "CoreMinimal.h"
#include "AnimInstanceControllerEvent.generated.h"

UENUM()
enum class EAnimInstanceControllerEvent : uint8 {
	OnBeforeDisengaged,
	OnAfterEnabled,
	Size
};
