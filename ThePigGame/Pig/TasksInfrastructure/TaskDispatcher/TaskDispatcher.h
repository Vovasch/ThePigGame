#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/EventHandler/TEventHandler.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "Containers/Queue.h"
#include "type_traits"
#include "TaskDispatcherEvent.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskData/Base/TaskDataBase.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskType/TaskType.h"
#include "TaskDispatcher.generated.h"

class UTaskBase;
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
	void OnEndTask(ETaskType taskType);

	const UTaskBase* GetTaskByType(ETaskType type);

	private:
	void TryStartNewTask();
	void OnTaskStarted(ETaskType taskType);

	UTaskBase* GetTaskByTypeInner(ETaskType taskType);
	UTaskBase* GetCurrentInProgressTask();
	
	template<typename TaskType>
	void CreateTask() {
		static_assert(std::is_base_of_v<UTaskBase, TaskType>, "TaskType must be derived from UTaskBase");

		auto task = TStrongObjectPtr(NewObject<TaskType>());
		m_vAllTasks[(uint32)task->GetTaskType()] = task;
	}

	enum class ETaskState {
		None,
		InQueue,
		InProgress
	};

	private:
	// tail is next to start or current in progress
	TQueue<ETaskType> m_xTaskQue;

	TStaticArray<TStrongObjectPtr<UTaskBase>, (int32)ETaskType::Size> m_vAllTasks{InPlace, nullptr};

	TStaticArray<ETaskState, (int32)ETaskType::Size> m_vTaskState {InPlace, ETaskState::None};

};