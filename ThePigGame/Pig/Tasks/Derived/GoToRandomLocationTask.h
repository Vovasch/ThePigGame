#pragma once
#include "../Base/InterruptibleTask.h"
#include "GoToRandomLocationTask.generated.h"

UCLASS()
class THEPIGGAME_API UGoToRandomLocationTask : public UInterruptibleTask {
	GENERATED_BODY()

	public:
	UGoToRandomLocationTask();

	public:
	virtual void Start() override;
	virtual void Interrupt() override;
	virtual void OnEnd() override;
};