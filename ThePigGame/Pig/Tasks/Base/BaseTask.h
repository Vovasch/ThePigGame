#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/EventHandler/TEventHandler.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "TaskEvent.h"
#include "../Derived/TaskType.h"
#include "BaseTask.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(TaskLog, Log, All)

UCLASS()
class THEPIGGAME_API UBaseTask : public UObject, public TEventHandler<ETaskEvent>, public ICachedPigDataUser {
	GENERATED_BODY()

	public:
	virtual void Start();
	virtual void Complete();
	virtual void Fail();
	virtual void Tick(float delta);

	public:
	ETaskType GetTaskType();

	public:
	bool IsInProgress();

	protected:
	virtual void OnEnd();

	protected:
	ETaskType m_xTaskType = ETaskType::None;

	private:
	bool m_bInProgress = false;
};