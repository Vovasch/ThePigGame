#include "PigSleepingState.h"

UPigSleepingState::UPigSleepingState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {}

EPigStates UPigSleepingState::StateType() const {
	return EPigStates::Sleeping;
}
