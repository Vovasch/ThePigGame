#pragma once

#include "TaskBase.h"
#include "InterruptibleTask.generated.h"

UCLASS()
class THEPIGGAME_API UInterruptibleTask : public UTaskBase {
	GENERATED_BODY()

	public:
	virtual void Interrupt();

};