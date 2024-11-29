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
	UConsumingController();

	public:
	virtual void Tick(float delta) override;

	public:
	const ConsumeProperty* GetBellyful();
	const ConsumeProperty* GetHydrated();

	public:
	virtual void InitProperties() override;

	public:
	void StartConsuming(TWeakObjectPtr<UConsumeSpotComponent> consumeSpot);

	public:
	EConsumeSourceType GetOccupiedSpotTypeChecked();

	private:
	void AddGoToConsumeSpotTask(EConsumeSourceType consumeType);
	void TryConsumeLater(EConsumeSourceType sourceType);

	private:
	void EndConsuming();

	private:
	void ProcessTick();
	void ProcessConsumingState();

	private:
	ConsumeProperty& GetPropertyByConsumeType(EConsumeSourceType sourceType);

	private:
	TMap<EConsumeSourceType, ConsumeProperty> m_mConsumeProperties;

	private:
	// if !IsExplicitlyNull => consuming is in progress
	TWeakObjectPtr<UConsumeSpotComponent> m_pOccupiedSpot = nullptr;
	TStaticArray<bool, uint32(EConsumeSourceType::Size)> m_vWaitingForSpot{InPlace, false};
};