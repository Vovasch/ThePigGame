#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/EventHandler/TEventHandler.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "Containers/Queue.h"
#include "type_traits"
#include "TaskDispatcherEvent.h"
#include "ThePigGame/Pig/Tasks/TaskType/TaskType.h"
#include "TaskDispatcher.generated.h"

class UTaskDataBase;
class UBaseTask;
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
	void AddTask(ETaskType taskType, TStrongObjectPtr<const UTaskDataBase> data=nullptr);

	public:
	void Tick(float delta);

	public:
	// todo make friend in UTaskBase. This should not be public
	void OnEndTask(ETaskType taskType);

	const UBaseTask* GetTaskByType(ETaskType type);

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

	protected:
	UBaseTask* GetTaskByTypeInner(ETaskType taskType);
	UBaseTask* GetCurrentInProgressTask();

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

	TStaticArray<ETaskState, (int32)ETaskType::Size> m_vTaskState;

};