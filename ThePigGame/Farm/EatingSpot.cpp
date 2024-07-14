#include "EatingSpot.h"

#include "Trough.h"

UEatingSpot::UEatingSpot(const FVector& location, bool available) : m_xLocation(location), m_bAvailable(available) {}

bool UEatingSpot::IsAvailable() { 
	return m_bAvailable;
}

void UEatingSpot::SetAvailable(bool isAvailable) {
	m_bAvailable = isAvailable;
	OnEvent(m_bAvailable ? EEatingSpotEvent::Freed : EEatingSpotEvent::Occupied);
}

void UEatingSpot::SetOwnerTrough(ATrough* trough) {
	m_pTroughOwner = trough;
}

ATrough* UEatingSpot::GetOwnerTrough() {
	return m_pTroughOwner.Get();
}

const FVector& UEatingSpot::GetLocation() {
	return m_xLocation;
}

void UEatingSpot::SetLocation(const FVector& loc) {
	m_xLocation = loc;
}