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

	static inline constexpr uint32 s_uConsumeTypesAmount = uint32(EConsumeSourceType::Size);

	public:
	virtual void Tick(float delta) override;

	public:
	const Consume* GetBellyful();

	public:
	virtual void InitProperties() override;

	public:
	void StartConsuming(TWeakObjectPtr<UConsumeSpotComponent> consumeSpot);
	void OnNoConsumeSpotAvailable(EConsumeSourceType sourceType);

	public:
	TWeakObjectPtr<const UConsumeSpotComponent> GetOccupiedSpot();

	private:
	void AddGoToConsumeSpotTask(EConsumeSourceType consumeType);
	void TryConsumeLater(EConsumeSourceType sourceType);

	private:
	void EndConsuming();

	private:
	void ProcessTick();
	void ProcessConsumingState();
	void ProcessNonConsumingState();

	private:
	Consume& GetPropertyByConsumeType(EConsumeSourceType sourceType);

	private:
	TStaticArray<Consume, s_uConsumeTypesAmount> m_vConsumeProperties {};

	private:
	// if !IsExplicitlyNull => consuming is in progress
	TWeakObjectPtr<UConsumeSpotComponent> m_pOccupiedSpot = nullptr;
	TStaticArray<bool, s_uConsumeTypesAmount> m_vWaitingForSpot{InPlace, false};
};