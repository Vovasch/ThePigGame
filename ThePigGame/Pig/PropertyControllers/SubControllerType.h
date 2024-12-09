#pragma once

#include "CoreMinimal.h"
#include "SubControllerType.generated.h"

UENUM()
enum class ESubControllerType : uint8 {
	Age,
	Consuming,
	Sleeping,
	Weight,
	Misc,
	Size
};
