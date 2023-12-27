#pragma once

#include "CoreMinimal.h"
#include "TBaseTask.h"
#include "TaskEvent.h"
#include "TaskType.h"
#include "Containers/Queue.h"
#include "type_traits"
#include "../../Utils/EventHandler/TEventHandler.h"
#include "TaskDispatcherEvent.h"
#include "TaskDispatcher.generated.h"

class APig;

UCLASS()
class THEPIGGAME_API UTaskDispatcher : public UObject, public TEventHandler<ETaskDispatcherEvent, ETaskType> {
	GENERATED_BODY()

	public:
	UTaskDispatcher();

	public:
	void SetPigOwner(APig*);
	APig* GetPigOwner();

	public:
	void AddTask(ETaskType taskType);

	public:
	void Tick(float delta);

	public:
	TSharedPtr<TBaseTask> GetTaskByType(ETaskType taskType);

	public:
	TSharedPtr<TBaseTask> GetCurrentInProgressTask();

	protected:
	void OnStartTask(ETaskType taskType);
	void OnEndTask(ETaskType taskType);

	protected:
	template<typename TaskType>
	void CreateTask() {
		static_assert(std::is_base_of_v<TBaseTask, TaskType>, "TaskType must be derived from TBaseTask");

		auto task = MakeShared<TaskType>(this);
		task->Subscribe(ETaskEvent::End, [this](ETaskType taskType) { this->OnEndTask(taskType); });
		m_vAllTasks[(uint32)task->GetTaskType()] = { task, ETaskState::None };
	}

	enum class ETaskState {
		None,
		InQueue,
		InProgress
	};


	protected:
	struct TTaskData {
		TSharedPtr<TBaseTask> Task = nullptr;
		ETaskState State = ETaskState::None;
	};

	protected:
	// tail is current in progress
	TQueue<ETaskType> m_xTaskQue;
	TStaticArray<TTaskData, (int32)ETaskType::Size> m_vAllTasks;

	protected:
	APig* m_pPigOwner = nullptr;


};