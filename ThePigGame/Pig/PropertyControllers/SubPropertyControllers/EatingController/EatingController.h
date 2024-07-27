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

	public:
	const Bellyful* GetBellyfull();

	public:
	UEatingSpot* GetTargetEatingSpot();
	void SetTargetEatingSpot(UEatingSpot* EatingSpot);

	public:
	void SetWaitingForEatingSpot(bool isWaiting);

	public:
	virtual void InitProperties() override;

	protected:
	void StartEating();
	void EndEating();

	protected:
	void ProcessNonEatingState();
	void ProcessEatingState();

	protected:
	Bellyful m_xBellyful;

	TWeakObjectPtr<UEatingSpot> CurrentEatingSpot = nullptr;

	protected:
	bool m_bIsWaitingForEatingSpot = false;

};