#pragma once

#include "TBaseTask.h"

class TGoToSleepTask : public TBaseTask {
	// main idea of this task
	// pig that got task to sleep tries to find any another pig
	// that has already started sleeping in order to lay down near her
	// if pig has found another sleepng pig to sleep near,
	// she goes to here
	// but while pig is going to another sleeping pig this pig can
	// wake up. so we subscribe to such event
	// if such event has been fired we do everything from the beginning.
	// if pig that want to sleep haven't found any another pig that 
	// has already started sleeping, she to goes random place inside sleeping area
	// and while going there pig is checking if any another pig started sleeping

	// TODO:
	// try find any another sleeping pig while going to sleeping area

	public:
	TGoToSleepTask();

	public:
	virtual void Start() override;

	public:
	virtual void Complete() override;

	public:
	virtual void Tick(float delta) override;

	protected:
	virtual void OnEnd() override;

	protected:
	void FindPlaceForSleeping();

	protected:
	void OnAnotherSleepingPigEndedSleeping();

	protected:
	void UnsubscribeFromAnotherSleepingPig();

	protected:
	void OnFailedToReachSleepingPlace();

	protected:
	APig* m_pAnotherSleepingPig = nullptr;

};