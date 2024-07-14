#include "PropertyControllerBase.h"

#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"


void UPropertyControllerBase::Init(USupremePropertyController* owner) {
	m_pOwnerController = owner;
}

APig* UPropertyControllerBase::GetPig() {
	return m_pOwnerController->GetPig();
}

void UPropertyControllerBase::Tick(float delta) {
	TickProviderTick(delta);
}

void UPropertyControllerBase::InitProperties() {
	
}

void UPropertyControllerBase::InitServiceProperties() {
	
}