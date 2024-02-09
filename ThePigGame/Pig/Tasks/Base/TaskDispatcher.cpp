#include "TaskDispatcher.h"

#include "InterruptibleTask.h"
#include "../Derived/GoToEatTask.h"
#include "../Derived/GoToSleepTask.h"
#include "../Derived/GoToRandomLocationTask.h"
#include "../../PigStateMachine/PigStateMachine.h"
#include "../../PigStateMachine/PigDefaultState.h"

UTaskDispatcher::UTaskDispatcher() {
	m_vAllTasks.AddDefaulted((uint32)ETaskType::Size);
	CreateTask<UGoToEatTask>();
	CreateTask<UGoToSleepTask>();
	CreateTask<UGoToRandomLocationTask>();
}

void UTaskDispatcher::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);
	for(auto& task : m_vAllTasks) {
		if(task) task->Init(pig);
	}

	for(auto& data : m_vTaskData) {
		data = ETaskState::None;
	}

	GetStateMachine()->GetState(EPigStates::Default)->Subscribe(this, EStateEvent::Start, [this]() {
		TryStartNewTask();
	});

}

void UTaskDispatcher::OnTaskStarted(ETaskType taskType) {
	m_vTaskData[uint32(taskType)] = ETaskState::InProgress;
	OnEvent(ETaskDispatcherEvent::TaskStarted, taskType);
}

void UTaskDispatcher::OnEndTask(ETaskType taskType) {
	auto currentTask = GetCurrentInProgressTask();
	if(!currentTask || currentTask->GetTaskType()!=taskType) return;

	m_xTaskQue.Pop();
	m_vTaskData[uint32(taskType)] = ETaskState::None;
	OnEvent(ETaskDispatcherEvent::TaskFinished, currentTask->GetTaskType());
	
	TryStartNewTask();
}

void UTaskDispatcher::AddTask(ETaskType taskType) {
	auto& taskData = m_vTaskData[(uint32)taskType];
	if(taskData != ETaskState::None) return;

	m_xTaskQue.Enqueue(taskType);
	taskData = ETaskState::InQueue;

	TryStartNewTask();
}

void UTaskDispatcher::Tick(float delta) {
	if(auto currentTask = GetCurrentInProgressTask()) {
		currentTask->Tick(delta);
	}
}

UBaseTask* UTaskDispatcher::GetTaskByType(ETaskType taskType) {
	if(taskType==ETaskType::None || taskType == ETaskType::Size) return nullptr;
	return m_vAllTasks[(uint32)taskType];
}

UBaseTask* UTaskDispatcher::GetCurrentInProgressTask() {
	if(m_xTaskQue.IsEmpty()) return nullptr;


	auto potentialInProgress = *m_xTaskQue.Peek();

	if(m_vTaskData[(uint32)potentialInProgress]==ETaskState::InProgress) return GetTaskByType(potentialInProgress);

	return nullptr;
}

void UTaskDispatcher::TryStartNewTask() {
	if(m_xTaskQue.IsEmpty()) return;
	if(GetStateMachine()->GetCurrentStateType() != EPigStates::Default) return;
	if(auto inProgressTask = GetCurrentInProgressTask()) {		
		if(auto asInterruptible = Cast<UInterruptibleTask>(inProgressTask)) {
			asInterruptible->Interrupt();
		} else {
			return;
		}
	}

	// if interruptible task is next in queue,
	// it should not be started if there are other task after it in que.
	// iterate through all task loop looking for non-interruptible task
	// or until there is only one task left in queue.
	auto taskTypeToStart = *m_xTaskQue.Peek();
	auto taskToStart = GetTaskByType(taskTypeToStart);
	auto taskAsInterruptible = Cast<UInterruptibleTask>(taskToStart);

	while(taskAsInterruptible) {
		m_vTaskData[(uint32)taskTypeToStart] = ETaskState::None;
		m_xTaskQue.Pop();
		
		if(m_xTaskQue.IsEmpty()) {
			m_xTaskQue.Enqueue(taskTypeToStart);
			break;
		}

		taskTypeToStart = *m_xTaskQue.Peek();
		taskToStart = GetTaskByType(taskTypeToStart);
		taskAsInterruptible = Cast<UInterruptibleTask>(taskToStart);
	}

	taskToStart->Start();
	OnTaskStarted(taskTypeToStart);
}
