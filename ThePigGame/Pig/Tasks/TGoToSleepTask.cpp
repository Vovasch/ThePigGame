#include "TGoToSleepTask.h"

TGoToSleepTask::TGoToSleepTask(UTaskDispatcher* owner) : TBaseTask(owner) {
	m_xTaskType = ETaskType::GoToSleep;
}