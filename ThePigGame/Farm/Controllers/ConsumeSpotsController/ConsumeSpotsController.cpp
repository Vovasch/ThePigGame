#include "ConsumeSpotsController.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSource.h"
#include "ThePigGame/Farm/Components/ConsumeSpotComponent/ConsumeSpotComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ConsumeSpotsControllerLog, Log, All)

void UConsumeSpotsController::AddConsumeSpot(UConsumeSpotComponent* consumeSpot) {
	FreeSpots[uint32(consumeSpot->GetSpotType())].Add(consumeSpot);
}

bool UConsumeSpotsController::TryOccupySpot(UConsumeSpotComponent* consumeSpot, const APig* pig) {
	if(!ValidateSpot(consumeSpot)) return false;

	if(!consumeSpot->IsAvailable()) return false;

	auto type = uint32(consumeSpot->GetSpotType());

	FreeSpots[type].Remove(consumeSpot);
	OccupiedSpots[type].Add(consumeSpot, pig);

	return true;
}

bool UConsumeSpotsController::TryFreeSpot(UConsumeSpotComponent* consumeSpot, const APig* pig) {
	if(!ValidateSpot(consumeSpot)) return false;

	if(!IsOccupiedBy(consumeSpot, pig)) return false;

	auto type = uint32(consumeSpot->GetSpotType());

	OccupiedSpots[type].Remove(consumeSpot);
	FreeSpots[type].Add(consumeSpot);

	return true;
}

bool UConsumeSpotsController::IsSpotAvailable(const UConsumeSpotComponent* consumeSpot) {
	//todo add validate spot
	return FreeSpots[uint32(consumeSpot->GetSpotType())].Contains(consumeSpot);
}

bool UConsumeSpotsController::IsOccupiedBy(const UConsumeSpotComponent* consumeSpot, const APig* consumer) {
	// todo add validate spot
	if(!consumer) return false;
	if(IsSpotAvailable(consumeSpot)) return false;

	auto type = uint32(consumeSpot->GetSpotType());
	auto result = OccupiedSpots[type].Find(consumeSpot);

	return *result == consumer;
}

UConsumeSpotComponent* UConsumeSpotsController::GetOccupiedBy(const APig* consumer, EConsumeSourceType type) {
	// TODO check if pis is from the same farm
	return *OccupiedSpots[uint32(type)].FindKey(consumer);
}

UConsumeSpotComponent* UConsumeSpotsController::GetNearestFreeSpot(FVector location, EConsumeSourceType type) {
	// todo implement

	auto& freeSpots = FreeSpots[uint32(type)];

	if(freeSpots.IsEmpty()) {
		return nullptr;	
	}

	return freeSpots[0];
}

bool UConsumeSpotsController::ValidateSpot(UConsumeSpotComponent* consumeSpot) {
	if(!consumeSpot) {
		ErrorOnInvalidSpot(TEXT("Consume spot is nullptr"));
		return false;
	}

	if(consumeSpot->GetConsumeSourceOwner()->GetOwnerFarm()!=GetFarm()) {
		ErrorOnInvalidSpot(TEXT("Consume spot from a different farm"));
		return false;
	}

	return true;
}

void UConsumeSpotsController::ErrorOnInvalidSpot(const FString& explanation) {
	UE_LOG(ConsumeSpotsControllerLog, Fatal, TEXT("Invalid consume spot. Reason: %s"), *explanation);
}