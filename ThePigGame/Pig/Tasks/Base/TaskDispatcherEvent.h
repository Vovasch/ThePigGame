#pragma once

#include "CoreMinimal.h"
#include "TaskDispatcherEvent.generated.h"	

UENUM(BlueprintType)
enum class ETaskDispatcherEvent : uint8 {
	TaskStarted,
	TaskFinished,
	Size
};