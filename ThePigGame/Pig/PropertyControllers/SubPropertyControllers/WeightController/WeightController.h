#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/PropertyControllerBase.h"
#include "WeightController.generated.h"

UCLASS()
class THEPIGGAME_API UWeightController : public UPropertyControllerBase {
	GENERATED_BODY()

	public:
	const MaxWeight* GetMaxWeight();
	const CriticalWeight* GetCriticalWeight();
	const Weight* GetWeight();

	public:
	virtual void Tick(float delta) override;

	protected:
	void CheckBellyful();

	public:
	virtual void InitServiceProperties() override;
	virtual void InitProperties() override;

	protected:
	MaxWeight m_xMaxWeight;
	CriticalWeight m_xCriticalWeight;
	Weight m_xWeight;

	float m_fWeightDeltaPerTick = 0.f;

};