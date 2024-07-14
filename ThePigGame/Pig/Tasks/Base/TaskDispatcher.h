#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/EventHandler/TEventHandler.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "BaseTask.h"
#include "TaskEvent.h"
#include "../Derived/TaskType.h"
#include "Containers/Queue.h"
#include "type_traits"
#include "TaskDispatcherEvent.h"
#include "TaskDispatcher.generated.h"

class APig;

UCLASS()
class THEPIGGAME_API UTaskDispatcher : public UObject, public TEventHandler<ETaskDispatcherEvent, ETaskType>, public ICachedPigDataUser {
	GENERATED_BODY()

	// Simultaneously only one task can be executed
	// Tasks can't start during the non DefaultPigState
	// So it's ok not to have any InProgress task in queue.

	public:
	UTaskDispatcher();

	virtual void Init(APig* pig) override;

	public:
	void AddTask(ETaskType taskType);

	public:
	void Tick(float delta);

	public:
	UBaseTask* GetTaskByType(ETaskType taskType);

	public:
	UBaseTask* GetCurrentInProgressTask();

	public:
	void OnEndTask(ETaskType taskType);

	protected:
	void TryStartNewTask();

	protected:
	void OnTaskStarted(ETaskType taskType);

	protected:
	template<typename TaskType>
	void CreateTask() {
		static_assert(std::is_base_of_v<UBaseTask, TaskType>, "TaskType must be derived from UBaseTask");

		auto task = NewObject<TaskType>();
		m_vAllTasks[(uint32)task->GetTaskType()] = task;
	}

	enum class ETaskState {
		None,
		InQueue,
		InProgress
	};

	protected:
	// tail is next to start or current in progress
	TQueue<ETaskType> m_xTaskQue;

	UPROPERTY()
	TArray<UBaseTask*> m_vAllTasks;

	TStaticArray<ETaskState, (int32)ETaskType::Size> m_vTaskData;

};