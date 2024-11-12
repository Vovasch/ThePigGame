#pragma once

#include "../Base/BaseTask.h"
#include "GoToSleepTask.generated.h"

UCLASS()
class THEPIGGAME_API UGoToSleepTask : public UBaseTask {
	// main idea of this task
	// pig that got task to sleep tries to find any another pig
	// that has already started sleeping in order to lay down near her
	// if pig has found another sleeping pig to sleep near,
	// she goes here
	// but while pig is going to another sleeping pig this pig can
	// wake up. so we subscribe to such event
	// if such event has been fired we do everything from the beginning.
	// if pig that want to sleep haven't found any another pig that 
	// has already started sleeping, she to goes random place inside sleeping area
	// and while going there pig is checking if any another pig started sleeping

	GENERATED_BODY()

	protected:
	enum class ESleepingSpotType {
		None,
		AnotherPig,
		RandomPlace
	};

	public:
	ETaskType GetTaskType() override;

	public:
	virtual void Start() override;

	public:
	virtual void Complete() override;

	public:
	virtual void Tick(float delta) override;

	protected:
	virtual void OnEnd() override;

	protected:
	void FindPlaceForSleeping();

	protected:
	void OnAnotherSleepingPigEndedSleeping();

	protected:
	void UnsubscribeFromAnotherSleepingPig();

	protected:
	void OnFailedToReachSleepingPlace();

	protected:
	TWeakObjectPtr<APig> m_pAnotherSleepingPig = nullptr;

	protected:
	ESleepingSpotType m_xSleepingSpotType = ESleepingSpotType::None;

};