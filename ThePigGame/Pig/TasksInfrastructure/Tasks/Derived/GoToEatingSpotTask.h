#pragma once

#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Base/GoToConsumeSpotTask.h"
#include "GoToEatingSpotTask.generated.h"

UCLASS()
class UGoToEatingSpotTask : public UGoToConsumeSpotTask {
	GENERATED_BODY()

	public:
	ETaskType GetTaskType() override;
};
