#pragma once

#include "TBaseTask.h"

class TGoToSleepTask : public TBaseTask {

	public:
	TGoToSleepTask();

	public:
	virtual void Start() override;
	virtual void GoToSleepingPlace();

};