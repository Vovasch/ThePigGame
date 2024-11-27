#pragma once

#include "GoToConsumeSpotTask.h"
#include "GoToDrinkingSpotTask.generated.h"

UCLASS()
class UGoToDrinkingSpotTask : public UGoToConsumeSpotTask {
	GENERATED_BODY()

	public:
	ETaskType GetTaskType() override;
};
