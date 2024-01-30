#include "PigEatingState.h"
#include "../Pig.h"

UPigEatingState::UPigEatingState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	SetStateType(EPigStates::Eating);
}

void UPigEatingState::Tick(float delta) {

	auto pig = GetPig();

	if(pig->GetCurrentBellyful() >= pig->GetBellyfulLevelToStopEating()) {
		pig->StopEating();
	}
	
}

