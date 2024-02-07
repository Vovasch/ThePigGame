#pragma once

#include "../Base/TBaseTask.h"

class UEatingSpot;

class TGoToEatTask : public TBaseTask {
	
	public:
	TGoToEatTask();

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