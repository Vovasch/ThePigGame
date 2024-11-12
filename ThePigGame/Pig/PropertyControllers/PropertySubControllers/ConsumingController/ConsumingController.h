#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/PropertySubControllerBase.h"
#include "ConsumingController.generated.h"

class UConsumeSpotComponent;
class ATrough;

UCLASS()
class THEPIGGAME_API UConsumingController : public UPropertySubControllerBase {
	GENERATED_BODY()

	public:
	virtual void Tick(float delta) override;

	public:
	const Bellyful* GetBellyful();

	public:
	virtual void InitProperties() override;

	public:
	void StartConsuming(TWeakObjectPtr<UConsumeSpotComponent> consumeSpot);
	void OnNoConsumeSpotAvailable(EConsumeSourceType sourceType);

	public:
	TWeakObjectPtr<const UConsumeSpotComponent> GetOccupiedSpot();

	private:
	void AddGoToConsumeSpotTask(EConsumeSourceType consumeType);

	private:
	void EndConsuming();

	private:
	void TryConsumeLater(EConsumeSourceType sourceType);

	private:
	void ProcessTick();
	void ProcessConsumingState();
	void ProcessNonConsumingState();

	private:
	void OnRetryGoToConsumeSpot(EConsumeSourceType sourceType);

	private:
	Bellyful m_xBellyful;

	private:
	// if !IsExplicitlyNull => consuming is in progress
	TWeakObjectPtr<UConsumeSpotComponent> m_pOccupiedSpot = nullptr;
	TStaticArray<bool, (uint32)EConsumeSourceType::Size> m_vWaitingForSpot{InPlace, false};
};