#pragma once

#include "CoreMinimal.h"
#include "TaskEvent.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskType/TaskType.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskData/Base/TaskDataBase.h"
#include "ThePigGame/Utils/EventHandler/TEventHandler.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "TaskBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TaskLog, Log, All)

UCLASS(Abstract)
class THEPIGGAME_API UTaskBase : public UObject, public TEventHandler<ETaskEvent>, public ICachedPigDataUser {
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

	virtual bool IsValidData();

	private:
	TStrongObjectPtr<const UTaskDataBase> m_pTaskData = nullptr;
	bool m_bInProgress = false;
};