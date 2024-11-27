#pragma once

#include "CoreMinimal.h"
#include "PigPropertyType.generated.h"

/// Also add new property in USupremePropertyController
///

UENUM()
enum class EPigPropertyType : uint8 {
	Age,
	IsAdult,
	Bellyful,
	// todo rename into hydrated
	Thirst,
	Energy,
	MaxWeight,
	CriticalWeight,
	Weight,
	Scale,
	Morph,
	Size
};
