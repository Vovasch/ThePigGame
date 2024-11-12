#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/PropertyControllerBase.h"
#include "SleepingController.generated.h"

UCLASS()
class THEPIGGAME_API USleepingController : public UPropertyControllerBase {
	GENERATED_BODY()

	public:
	const Energy* GetEnergy();

	public:
	virtual void Tick(float delta) override;

	public:
	void GoToSleep();
	void WakeUp();

	public:
	virtual void InitProperties() override;

	protected:
	void CheckEnergyLevel();

	protected:
	Energy m_xEnergy;

};