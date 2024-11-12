#pragma once

#include "CoreMinimal.h"
#include "TaskEvent.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskType/TaskType.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskData/Base/TaskDataBase.h"
#include "ThePigGame/Utils/EventHandler/TEventHandler.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "BaseTask.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TaskLog, Log, All)

// todo rename into UTaskBase
UCLASS(Abstract)
class THEPIGGAME_API UBaseTask : public UObject, public TEventHandler<ETaskEvent>, public ICachedPigDataUser {
	GENERATED_BODY()

	public:
	virtual void Start();
	virtual void Complete();
	virtual void Fail();
	virtual void Tick(float delta);

	public:
	virtual ETaskType GetTaskType() PURE_VIRTUAL("Not implemented", return ETaskType::Size;)

	public:
	bool IsInProgress();

	public:
	void SetTaskData(TStrongObjectPtr<const UTaskDataBase> taskData);

	protected:
	virtual void OnEnd();

	protected:
	TStrongObjectPtr<const UTaskDataBase> m_pTaskData = nullptr;

	private:
	bool m_bInProgress = false;
};