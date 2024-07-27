#include "TroughsController.h"

#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Farm/Components/Trough/Trough.h"


// get all troughs to be able to control them.
void UTroughsController::Init() {

	TArray<AActor*> actors;
	GetFarm()->GetAllChildActors(actors);

	m_iAvailableEatingSpots = m_iTotalEatingSpots = 0;

	for(auto actor : actors) {
		if(auto trough = Cast<ATrough>(actor)) {
			m_vTroughs.Add(trough);

			auto& eatingSpots = trough->GetAllEatingSpots();

			m_iTotalEatingSpots += eatingSpots.Num();

			for(auto eatingSpot : eatingSpots) {
				eatingSpot->Subscribe(this, EEatingSpotEvent::Occupied, [this]() {
						--m_iAvailableEatingSpots;
				});

				eatingSpot->Subscribe(this, EEatingSpotEvent::Freed, [this]() {
						auto noneWereAvailable = m_iAvailableEatingSpots == 0;

						++m_iAvailableEatingSpots;
						if(noneWereAvailable) OnEvent(ETroughsControllerEvent::EatingSpotFreed);

				});
			}
		}
	}

	m_iAvailableEatingSpots = m_iTotalEatingSpots;

}

UEatingSpot* UTroughsController::GetAvailableEatingSpot() {
	for(auto& trough : m_vTroughs) {
		auto eatingSpot = trough->GetAvailableEatingSpot();
		if(eatingSpot) return eatingSpot;
	}

	return nullptr;
}