#pragma once

#include "CoreMinimal.h"
#include "Base/TaskDataBase.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "GoToConsumeSpotData.generated.h"

UCLASS()
class UGoToConsumeSpotData : public UTaskDataBase {
	GENERATED_BODY()

	public:
	EConsumeSourceType ConsumeType = EConsumeSourceType::Size;
};