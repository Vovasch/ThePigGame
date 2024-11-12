#include "PropertySubControllerBase.h"

#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"


void UPropertySubControllerBase::Init(USupremePropertyController* owner) {
	m_pOwnerController = owner;
}

APig* UPropertySubControllerBase::GetPig() {
	return m_pOwnerController->GetPig();
}

void UPropertySubControllerBase::Tick(float delta) {
	TickProviderTick(delta);
}

void UPropertySubControllerBase::InitProperties() {
	
}

void UPropertySubControllerBase::InitServiceProperties() {
	
}