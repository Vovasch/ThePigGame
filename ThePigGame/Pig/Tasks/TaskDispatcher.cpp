#include "TaskDispatcher.h"

#include "TGoToEatTask.h"
#include "TGoToSleepTask.h"
#include "memory"

UTaskDispatcher::UTaskDispatcher() {
	CreateTask<TGoToEatTask>();
	CreateTask<TGoToSleepTask>();
}

void UTaskDispatcher::SetPigOwner(APig* owner) {
	m_pPigOwner = owner;
}

APig* UTaskDispatcher::GetPigOwner() {
	return m_pPigOwner;
}

void UTaskDispatcher::OnStartTask(ETaskType taskType) {
	m_vAllTasks[uint32(taskType)].State = ETaskState::InProgress;
	OnEvent(ETaskDispatcherEvent::TaskStarted, taskType);
}

void UTaskDispatcher::OnEndTask(ETaskType taskType) {
	auto currentTask = GetCurrentInProgressTask();
	if(!currentTask || currentTask->GetTaskType()!=taskType) return;

	// finish current task
	m_xTaskQue.Pop();
	m_vAllTasks[uint32(taskType)].State = ETaskState::None;
	OnEvent(ETaskDispatcherEvent::TaskFinished, currentTask->GetTaskType());
	
	// start new task if such in task queue.
	auto newCurrentTask = GetCurrentInProgressTask();
	if(newCurrentTask) {
		newCurrentTask->Start();
		OnStartTask(newCurrentTask->GetTaskType());
	}
}

void UTaskDispatcher::AddTask(ETaskType taskType) {
	auto& taskData = m_vAllTasks[(uint32)taskType];
	if(taskData.State!=ETaskState::None) return;

	auto startNow = m_xTaskQue.IsEmpty();
	m_xTaskQue.Enqueue(taskType);
	taskData.State = ETaskState::InQueue;
	auto newTask = GetTaskByType(taskType);

	if(startNow) {
		newTask->Start();
		OnStartTask(newTask->GetTaskType());
	}

}

void UTaskDispatcher::Tick(float delta) {
	if(auto currentTask = GetCurrentInProgressTask()) {
		currentTask->Tick(delta);
	}
}


TSharedPtr<TBaseTask> UTaskDispatcher::GetTaskByType(ETaskType taskType) {
	if(taskType==ETaskType::None || taskType == ETaskType::Size) return nullptr;
	return m_vAllTasks[(uint32)taskType].Task;
}

TSharedPtr<TBaseTask> UTaskDispatcher::GetCurrentInProgressTask() {
	if(m_xTaskQue.IsEmpty()) return nullptr;
	return GetTaskByType(*m_xTaskQue.Peek());
}
