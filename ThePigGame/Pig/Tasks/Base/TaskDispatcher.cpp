#include "TaskDispatcher.h"

#include "../Derived/TGoToEatTask.h"
#include "../Derived/TGoToSleepTask.h"
#include "../../PigStateMachine/PigStateMachine.h"
#include "../../PigStateMachine/PigDefaultState.h"

UTaskDispatcher::UTaskDispatcher() {
	CreateTask<TGoToEatTask>();
	CreateTask<TGoToSleepTask>();
}

void UTaskDispatcher::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);
	for(auto& taskData : m_vAllTasks) {
		if(taskData.Task) taskData.Task->Init(pig);
	}

	GetStateMachine()->GetState(EPigStates::Default)->Subscribe(this, EStateEvent::Start, [this]() {
		TryStartNewTask();
	});

}

void UTaskDispatcher::OnTaskStarted(ETaskType taskType) {
	m_vAllTasks[uint32(taskType)].State = ETaskState::InProgress;
	OnEvent(ETaskDispatcherEvent::TaskStarted, taskType);
}

void UTaskDispatcher::OnEndTask(ETaskType taskType) {
	auto currentTask = GetCurrentInProgressTask();
	if(!currentTask || currentTask->GetTaskType()!=taskType) return;

	m_xTaskQue.Pop();
	m_vAllTasks[uint32(taskType)].State = ETaskState::None;
	OnEvent(ETaskDispatcherEvent::TaskFinished, currentTask->GetTaskType());
	
	TryStartNewTask();
}

void UTaskDispatcher::AddTask(ETaskType taskType) {
	auto& taskData = m_vAllTasks[(uint32)taskType];
	if(taskData.State!=ETaskState::None) return;

	m_xTaskQue.Enqueue(taskType);
	taskData.State = ETaskState::InQueue;

	TryStartNewTask();
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


	auto potentialInProgress = *m_xTaskQue.Peek();

	if(m_vAllTasks[(uint32)potentialInProgress].State==ETaskState::InProgress) return GetTaskByType(potentialInProgress);

	return nullptr;
}

void UTaskDispatcher::TryStartNewTask() {
	if(m_xTaskQue.IsEmpty()) return;
	if(GetStateMachine()->GetCurrentStateType() != EPigStates::Default) return;
	if(auto inProgressTask = GetCurrentInProgressTask()) return;

	auto taskTypeToStart = *m_xTaskQue.Peek();

	GetTaskByType(taskTypeToStart)->Start();
	OnTaskStarted(taskTypeToStart);
}
