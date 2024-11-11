#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "ThePigGame/Farm/Controllers/FarmControllerBase/FarmControllerBase.h"
#include "ConsumeSpotsController.generated.h"

class APig;
class UConsumeSpotComponent;

UCLASS()
class UConsumeSpotsController : public UFarmControllerBase {
	GENERATED_BODY()

	using SpotStorage = TStaticArray<TArray<UConsumeSpotComponent*>, uint32(EConsumeSourceType::Size)>;
	using OccupiedSpotsStorage = TStaticArray<TMap<UConsumeSpotComponent*, const APig*>, uint32(EConsumeSourceType::Size)>;

	public:
	// todo make friend in UConsumeSource. This should not be public for everyone
	void AddConsumeSpot(UConsumeSpotComponent* consumeSpot);

	public:
	// true - successfully occupied spot; false - it has already been occupied.
	bool TryOccupySpot(UConsumeSpotComponent* consumeSpot, const APig* pig);

	// true - successfully freed spot; false - it has already been freed.
	// todo don't forget to free spot after pig is removed from farm
	bool TryFreeSpot(UConsumeSpotComponent* consumeSpot, const APig* pig);

	public:
	bool IsSpotAvailable(const UConsumeSpotComponent* consumeSpot);
	bool IsOccupiedBy(const UConsumeSpotComponent* consumeSpot, const APig* consumer);
	UConsumeSpotComponent* GetOccupiedBy(const APig* consumer, EConsumeSourceType type);

	public:
	UConsumeSpotComponent* GetNearestFreeSpot(FVector location, EConsumeSourceType type);

	private:
	// only checks if the spot is controlled by this controller
	bool ValidateSpot(UConsumeSpotComponent* consumeSpot);
	void ErrorOnInvalidSpot(const FString& explanation);

	private:
	SpotStorage FreeSpots;
	OccupiedSpotsStorage OccupiedSpots;
};