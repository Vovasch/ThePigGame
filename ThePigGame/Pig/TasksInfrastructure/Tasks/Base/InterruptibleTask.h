#pragma once

#include "BaseTask.h"
#include "InterruptibleTask.generated.h"

UCLASS()
class THEPIGGAME_API UInterruptibleTask : public UBaseTask {
	GENERATED_BODY()

	public:
	virtual void Interrupt();

};