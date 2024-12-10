#pragma once

#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Base/GoToConsumeSpotTask.h"
#include "GoToDrinkingSpotTask.generated.h"

UCLASS()
class UGoToDrinkingSpotTask : public UGoToConsumeSpotTask {
	GENERATED_BODY()

	public:
	ETaskType GetTaskType() override;
};
