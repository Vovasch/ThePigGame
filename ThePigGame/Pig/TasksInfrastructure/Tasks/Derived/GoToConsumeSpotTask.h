#pragma once

#include "../Base/TaskBase.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskType/TaskType.h"
#include "GoToConsumeSpotTask.generated.h"

class UGoToConsumeSpotData;
class UConsumeSpotComponent;

// todo move into base folder
UCLASS(Abstract)
class THEPIGGAME_API UGoToConsumeSpotTask : public UTaskBase {
	GENERATED_BODY()
	
	public:
	virtual void Start() override;
	virtual void OnEnd() override;
	virtual void Tick(float delta) override;

	public:
	ETaskType GetTaskType() override PURE_VIRTUAL("Not implemented", return ETaskType::None;)

	private:
	EConsumeSourceType GetSourceType();

	private:
	void TryGoToSpot();

	private:
	void OnMovementCompleted();

	private:
	void OnNoSpotAvailable();

	private:
	void UnsubscribeAll();

	private:
	TWeakObjectPtr<UConsumeSpotComponent> m_pTargetConsumeSpot = nullptr;
};