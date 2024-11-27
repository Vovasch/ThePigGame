#include "TaskDispatcher.h"
#include "../../PigStateMachine/PigStateMachine.h"
#include "../../PigStateMachine/PigDefaultState.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToConsumeSpotTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToDrinkingSpotTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToEatingSpotTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToRandomLocationTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToSleepTask.h"

UTaskDispatcher::UTaskDispatcher() {
	// todo make m_valltask a staticarray and add checking if all types of tasks has been created.
	m_vAllTasks.AddDefaulted((uint32)ETaskType::Size);
	CreateTask<UGoToEatingSpotTask>();
	CreateTask<UGoToDrinkingSpotTask>();
	CreateTask<UGoToSleepTask>();
	CreateTask<UGoToRandomLocationTask>();
}

void UTaskDispatcher::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);
	// todo here shall not be if(task) check this is stupid we shall have fatal error and not able to get to this stage of the program if any task is a nullptr
	for(auto& task : m_vAllTasks) {
		if(task) task->Init(pig);
	}

	for(auto& data : m_vTaskState) {
		data = ETaskState::None;
	}

	GetStateMachine()->GetState(EPigStates::Default)->Subscribe(this, EStateEvent::Start, [this]() {
		TryStartNewTask();
	});
}

void UTaskDispatcher::OnTaskStarted(ETaskType taskType) {
	// todo this seems to be unused
	OnEvent(ETaskDispatcherEvent::TaskStarted, taskType);
}

// todo this should not be public make such method in taskbase and make it friend here
void UTaskDispatcher::OnEndTask(ETaskType taskType) {
	auto currentTask = GetCurrentInProgressTask();
	if(!currentTask || currentTask->GetTaskType()!=taskType) return;

	m_xTaskQue.Pop();
	m_vTaskState[uint32(taskType)] = ETaskState::None;
	// todo this seems to be unused
	OnEvent(ETaskDispatcherEvent::TaskFinished, currentTask->GetTaskType());
	
	TryStartNewTask();
}

const UTaskBase* UTaskDispatcher::GetTaskByType(ETaskType taskType) {
	return GetTaskByTypeInner(taskType);
}

void UTaskDispatcher::AddTask(ETaskType taskType, TStrongObjectPtr<const UTaskDataBase> data) {
	// todo it's better to check this when adding task with data. just make function in UTaskBase::CheckMyData(data){return true}. then override where needed
	auto& taskState = m_vTaskState[uint32(taskType)];
	if(taskState != ETaskState::None) return;

	GetTaskByTypeInner(taskType)->SetTaskData(data);
	m_xTaskQue.Enqueue(taskType);
	taskState = ETaskState::InQueue;

	TryStartNewTask();
}

void UTaskDispatcher::Tick(float delta) {
	if(auto currentTask = GetCurrentInProgressTask()) {
		currentTask->Tick(delta);
	}
}

UTaskBase* UTaskDispatcher::GetTaskByTypeInner(ETaskType taskType) {
	if(taskType==ETaskType::None || taskType == ETaskType::Size) return nullptr;
	return m_vAllTasks[(uint32)taskType];
}

UTaskBase* UTaskDispatcher::GetCurrentInProgressTask() {
	if(m_xTaskQue.IsEmpty()) return nullptr;

	auto potentialInProgress = *m_xTaskQue.Peek();

	if(m_vTaskState[(uint32)potentialInProgress]==ETaskState::InProgress) return GetTaskByTypeInner(potentialInProgress);

	return nullptr;
}

void UTaskDispatcher::TryStartNewTask() {
	if(m_xTaskQue.IsEmpty()) return;
	if(GetStateMachine()->GetCurrentStateType() != EPigStates::Default) return;

	auto currentInProgress = GetCurrentInProgressTask();

	if(currentInProgress) {
		if(auto asInterruptible = Cast<UInterruptibleTask>(currentInProgress)) {
			// Interrupt will call OnEnd
			// OnEnd will call TryStartNewTask
			// That's why after interrupt goes return
			asInterruptible->Interrupt();
		}

		return;
	}

	// if interruptible task is next in queue,
	// it should not be started if there are other task after it in que.
	// iterate through all task loop looking for non-interruptible task
	// or until there is only one task left in queue.
	auto taskTypeToStart = *m_xTaskQue.Peek();
	auto taskToStart = GetTaskByTypeInner(taskTypeToStart);
	auto taskAsInterruptible = Cast<UInterruptibleTask>(taskToStart);

	while(taskAsInterruptible) {
		m_vTaskState[(uint32)taskTypeToStart] = ETaskState::None;
		m_xTaskQue.Pop();
		
		if(m_xTaskQue.IsEmpty()) {
			m_xTaskQue.Enqueue(taskTypeToStart);
			// todo maybe set some task state for task we've returned. in progress will be set later
			break;
		}

		taskTypeToStart = *m_xTaskQue.Peek();
		taskToStart = GetTaskByTypeInner(taskTypeToStart);
		taskAsInterruptible = Cast<UInterruptibleTask>(taskToStart);
	}

	m_vTaskState[uint32(taskTypeToStart)] = ETaskState::InProgress;
	taskToStart->Start();
	OnTaskStarted(taskTypeToStart);
}
