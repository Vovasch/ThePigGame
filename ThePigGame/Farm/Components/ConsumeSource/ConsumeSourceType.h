#pragma once

#include "CoreMinimal.h"
#include "ConsumeSourceType.generated.h"

UENUM(BlueprintType)
enum class EConsumeSourceType : uint8 {
	Eating,
	Drinking,
	Size
};