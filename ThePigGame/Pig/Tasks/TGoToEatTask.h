#pragma once

#include "TBaseTask.h"

class UEatingSpot;

class TGoToEatTask : public TBaseTask {
	
	public:
	TGoToEatTask();

	public:
	virtual void Start() override;
	void OnNoEatingSpotAvailable();
	void OnFailedToReachEatingSpot();
};