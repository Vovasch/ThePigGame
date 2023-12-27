#pragma once

#include "TBaseTask.h"

class UEatingSpot;

class TGoToEatTask : public TBaseTask {
	
	public:
	TGoToEatTask(UTaskDispatcher* owner);

	public:
	virtual void Start() override;
	virtual void Fail() override;
};