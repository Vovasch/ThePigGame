#include "ConsumeSpotComponent.h"
#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSource.h"
#include "ThePigGame/Farm/Controllers/ConsumeSpotsController/ConsumeSpotsController.h"

bool UConsumeSpotComponent::IsAvailable() const { 
	return GetConsumeSourceOwner()->GetOwnerFarm()->GetConsumeSpotsController()->IsSpotAvailable(this);
}

AConsumeSource* UConsumeSpotComponent::GetConsumeSourceOwner() const {
	return Cast<AConsumeSource>(GetOwner());
}

EConsumeSourceType UConsumeSpotComponent::GetSpotType() const {
	return GetConsumeSourceOwner()->GetConsumeType();
}

TOptional<float> UConsumeSpotComponent::TryConsumeOut(float amount, APig* consumer) {
	return GetConsumeSourceOwner()->TryConsumeOut(amount, consumer, this);
}
