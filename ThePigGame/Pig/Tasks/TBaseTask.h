#pragma once

#include "../../Utils/EventHandler/TEventHandler.h"
#include "TaskEvent.h"
#include "TaskType.h"
#include "TaskDispatcher.h"
//class UTaskDispatcher;

class TBaseTask : public TEventHandler<ETaskEvent, ETaskType> {

	public:
	TBaseTask(UTaskDispatcher* owner);

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

	protected:
	UTaskDispatcher* m_pTaskDispatcherOwner = nullptr;

	private:
	bool m_bInProgress = false;
};