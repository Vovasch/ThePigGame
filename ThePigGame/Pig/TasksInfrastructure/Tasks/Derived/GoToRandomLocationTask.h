#pragma once
#include "../Base/InterruptibleTask.h"
#include "GoToRandomLocationTask.generated.h"

UCLASS()
class THEPIGGAME_API UGoToRandomLocationTask : public UInterruptibleTask {
	GENERATED_BODY()

	public:
	ETaskType GetTaskType() override;

	public:
	void Start() override;
	void Interrupt() override;
	void OnEnd() override;
};