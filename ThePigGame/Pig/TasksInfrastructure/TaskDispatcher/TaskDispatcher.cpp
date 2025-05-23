#include "TaskDispatcher.h"
#include "../../PigStateMachine/PigStateMachine.h"
#include "../../PigStateMachine/PigDefaultState.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToDrinkingSpotTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToEatingSpotTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToRandomLocationTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Derived/GoToSleepTask.h"
#include "ThePigGame/Utils/Misc/TMiscUtils.h"

UTaskDispatcher::UTaskDispatcher() {
	CreateTask<UGoToEatingSpotTask>();
	CreateTask<UGoToDrinkingSpotTask>();
	CreateTask<UGoToSleepTask>();
	CreateTask<UGoToRandomLocationTask>();

	utils::VerifyContainerElements<ETaskType>(m_vAllTasks);
}

void UTaskDispatcher::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);

	// i = 1; because of None
	for(int32 i = 1; i < m_vAllTasks.Num(); ++i) {
		m_vAllTasks[i]->Init(pig);
	}

	GetStateMachine()->GetState(EPigStates::Default)->Subscribe(this, EStateEvent::Start, [this]() {
		TryStartNewTask();
	});
}

void UTaskDispatcher::OnTaskStarted(ETaskType taskType) {
	OnEvent(ETaskDispatcherEvent::TaskStarted, taskType);
}

void UTaskDispatcher::OnEndTask(ETaskType taskType) {
	auto currentTask = GetCurrentInProgressTask();
	if(!currentTask || currentTask->GetTaskType()!=taskType) return;

	m_xTaskQue.Pop();
	m_vTaskState[uint32(taskType)] = ETaskState::None;
	OnEvent(ETaskDispatcherEvent::TaskFinished, currentTask->GetTaskType());
	
	TryStartNewTask();
}

const UTaskBase* UTaskDispatcher::GetTaskByType(ETaskType taskType) {
	return GetTaskByTypeInner(taskType);
}

void UTaskDispatcher::AddTask(ETaskType taskType, TStrongObjectPtr<const UTaskDataBase> data) {
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
	return m_vAllTasks[(uint32)taskType].Get();
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
			m_vTaskState[(uint32)taskTypeToStart] = ETaskState::InQueue;
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
