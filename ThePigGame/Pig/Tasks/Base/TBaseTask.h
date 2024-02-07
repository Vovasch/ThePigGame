#pragma once

#include "../../../Utils/EventHandler/TEventHandler.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "TaskEvent.h"
#include "../Derived/TaskType.h"


class TBaseTask : public TEventHandler<ETaskEvent, ETaskType>, public ICachedPigDataUser {
	public:
	virtual void Start();
	virtual void Complete();
	virtual void Fail();
	virtual void Tick(float delta);

	public:
	ETaskType GetTaskType();

	protected:
	virtual void OnEnd();

	protected:
	ETaskType m_xTaskType = ETaskType::None;

	private:
	bool m_bInProgress = false;
};