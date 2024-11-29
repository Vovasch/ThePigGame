#include "ConsumeSpotsController.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSource.h"
#include "ThePigGame/Farm/Components/ConsumeSpotComponent/ConsumeSpotComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ConsumeSpotsControllerLog, Log, All)

void UConsumeSpotsController::AddConsumeSpot(UConsumeSpotComponent* consumeSpot) {
	FreeSpots[uint32(consumeSpot->GetSpotType())].Add(consumeSpot);
}

bool UConsumeSpotsController::TryOccupySpot(UConsumeSpotComponent* consumeSpot, const APig* pig) {
	ValidateSpot(consumeSpot);

	if(!consumeSpot->IsAvailable()) return false;

	auto type = uint32(consumeSpot->GetSpotType());

	FreeSpots[type].Remove(consumeSpot);
	OccupiedSpots[type].Add(consumeSpot, pig);

	return true;
}

bool UConsumeSpotsController::TryFreeSpot(UConsumeSpotComponent* consumeSpot, const APig* pig) {
	ValidateSpot(consumeSpot);

	if(!IsOccupiedBy(consumeSpot, pig)) return false;

	auto type = uint32(consumeSpot->GetSpotType());

	OccupiedSpots[type].Remove(consumeSpot);
	FreeSpots[type].Add(consumeSpot);

	return true;
}

bool UConsumeSpotsController::IsSpotAvailable(const UConsumeSpotComponent* consumeSpot) {
	ValidateSpot(consumeSpot);
	return FreeSpots[uint32(consumeSpot->GetSpotType())].Contains(consumeSpot);
}

bool UConsumeSpotsController::IsOccupiedBy(const UConsumeSpotComponent* consumeSpot, const APig* consumer) {
	ValidateSpot(consumeSpot);
	if(!consumer) return false;
	if(IsSpotAvailable(consumeSpot)) return false;

	auto type = uint32(consumeSpot->GetSpotType());
	auto result = OccupiedSpots[type].Find(consumeSpot);

	return *result == consumer;
}

UConsumeSpotComponent* UConsumeSpotsController::GetOccupiedBy(const APig* consumer, EConsumeSourceType type) {
	return *OccupiedSpots[uint32(type)].FindKey(consumer);
}

UConsumeSpotComponent* UConsumeSpotsController::GetNearestFreeSpot(FVector location, EConsumeSourceType type) {
	auto& freeSpots = FreeSpots[uint32(type)];

	if(freeSpots.IsEmpty()) {
		return nullptr;	
	}

	auto min = FVector::DistSquared(location, freeSpots[0]->GetComponentLocation());
	auto result = freeSpots[0];

	for(auto spot : freeSpots) {
		auto dist = FVector::DistSquared(location, spot->GetComponentLocation());
		if(dist<min) {
			min = dist;
			result = spot;
		}
	}

	return result;
}

void UConsumeSpotsController::ValidateSpot(const UConsumeSpotComponent* consumeSpot) {
	if(!consumeSpot) {
		ErrorOnInvalidSpot(TEXT("Consume spot is nullptr"));
	}

	if(consumeSpot->GetConsumeSourceOwner()->GetOwnerFarm()!=GetFarm()) {
		ErrorOnInvalidSpot(TEXT("Consume spot from a different farm"));
	}
}

void UConsumeSpotsController::ErrorOnInvalidSpot(const FString& explanation) {
	UE_LOG(ConsumeSpotsControllerLog, Fatal, TEXT("Invalid consume spot. Reason: %s"), *explanation);
}