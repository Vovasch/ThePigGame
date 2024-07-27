#include "SleepingPigsController.h"

#include "ThePigGame/Pig/Pig.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/PigStateMachine/PigStates.h"
#include "ThePigGame/Utils/StateMachine/StateEvent.h"

void USleepingPigsController::RegisterPig(APig* pig) {
	pig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Subscribe(this, EStateEvent::Start, [this, pig]() {
		OnPigStartedSleeping(pig);
	});

	pig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Subscribe(this, EStateEvent::End, [this, pig]() {
		OnPigEndedSleeping(pig);
	});

	// todo implement pig live phase controller
	/*pig->Subscribe(this, EPigEvent::RemovedFromFarm, [this, pig]() {
		m_pSleepingArea->OnPigRemovedFromFarm(pig);
	});*/
}

void USleepingPigsController::OnPigStartedSleeping(APig* pig) {
	m_vSleepingPigs.Add(pig);
}

void USleepingPigsController::RemovePig(APig* pig) {
	m_vSleepingPigs.Remove(pig);
}

void USleepingPigsController::OnPigEndedSleeping(APig* pig) {
	RemovePig(pig);
}

void USleepingPigsController::OnPigRemovedFromFarm(APig* pig) {
	RemovePig(pig);
}

APig* USleepingPigsController::GetAnySleepingPig() const {
	if(m_vSleepingPigs.IsEmpty()) return nullptr;
	return *m_vSleepingPigs.begin();
}