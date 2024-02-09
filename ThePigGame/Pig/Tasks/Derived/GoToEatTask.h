#pragma once

#include "../Base/BaseTask.h"
#include "GoToEatTask.generated.h"

class UEatingSpot;

UCLASS()
class THEPIGGAME_API UGoToEatTask : public UBaseTask {
	GENERATED_BODY()
	public:
	UGoToEatTask();

	public:
	virtual void Start() override;
	virtual void OnEnd() override;
	virtual void Tick(float delta) override;

	protected:
	void TryGoToEatingSpot();

	protected:
	void OnNoEatingSpotAvailable();

	protected:
	void UnsubscribeAll();
};