#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/PropertyControllerBase.h"
#include "EatingController.generated.h"

class UEatingSpot;
class ATrough;

UCLASS()
class THEPIGGAME_API UEatingController : public UPropertyControllerBase
{
	GENERATED_BODY()

	public:
	virtual void Tick(float delta) override;

	const Bellyful* GetBellyfull();

	void StartEating();
	void EndEating();

	UEatingSpot* GetTargetEatingSpot();
	void SetTargetEatingSpot(UEatingSpot* EatingSpot);

	void SetWaitingForEatingSpot(bool isWaiting);

	void CheckBellyfulLevel();

	public:
	virtual void InitProperties() override;

	protected:
	Bellyful m_xBellyful;

	protected:
	TWeakObjectPtr<ATrough> CurrentTrough = nullptr;
	TWeakObjectPtr<UEatingSpot> CurrentEatingSpot = nullptr;

	protected:
	bool m_bIsWaitingForEatingSpot = false;

};