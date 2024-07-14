#include "PigEatingState.h"
#include "../Pig.h"

UPigEatingState::UPigEatingState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	SetStateType(EPigStates::Eating);
}


