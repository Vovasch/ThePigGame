#pragma once

#include "../Base/BaseTask.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "GoToConsumeSpotTask.generated.h"

class UGoToConsumeSpotData;
class UConsumeSpotComponent;

UCLASS()
class THEPIGGAME_API UGoToConsumeSpotTask : public UBaseTask {
	GENERATED_BODY()
	
	public:
	virtual void Start() override;
	virtual void OnEnd() override;
	virtual void Tick(float delta) override;

	public:
	ETaskType GetTaskType() override;

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
	const UGoToConsumeSpotData* GetTaskDataChecked();

	private:
	TWeakObjectPtr<UConsumeSpotComponent> m_pTargetConsumeSpot = nullptr;
};