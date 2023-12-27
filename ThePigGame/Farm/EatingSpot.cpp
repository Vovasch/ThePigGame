#include "EatingSpot.h"

UEatingSpot::UEatingSpot(const FVector& location, bool available) : m_xLocation(location), m_bAvailable(available) {}

bool UEatingSpot::IsAvailable() { 
	return m_bAvailable;
}

void UEatingSpot::SetAvailable(bool isAvailable) {
	m_bAvailable = isAvailable;
	OnEvent(m_bAvailable ? EEatingSpotEvent::Freed : EEatingSpotEvent::Occupied);
}

const FVector& UEatingSpot::GetLocation() {
	return m_xLocation;
}

void UEatingSpot::SetLocation(const FVector& loc) {
	m_xLocation = loc;
}