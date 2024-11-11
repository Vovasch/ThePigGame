#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/EventHandler/TEventHandler.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "TaskEvent.h"
#include "ThePigGame/Pig/Tasks/TaskType/TaskType.h"
#include "../TaskData/Base/TaskDataBase.h"
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