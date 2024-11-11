#include "PigConsumingState.h"
#include "../Pig.h"

UPigConsumingState::UPigConsumingState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	SetStateType(EPigStates::Consuming);
}


