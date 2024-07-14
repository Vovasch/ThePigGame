#pragma once

#include "CoreMinimal.h"
#include "PigPropertyType.generated.h"

UENUM()
enum class EPigPropertyType : uint8 {
	Age,
	IsAdult,
	Bellyful,
	Energy,
	MaxWeight,
	CriticalWeight,
	Weight,
	Scale,
	Morph,
	Size
};
