#pragma once

#include "CoreMinimal.h"
#include "AgeControllerEvent.generated.h"

UENUM()
enum class EAgeControllerEvent : uint8 {
	BecomeAdult,
	BecomeOld,
	Size
};