#include "PigLayingDownState.h"

UPigLayingDownState::UPigLayingDownState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {}

EPigStates UPigLayingDownState::StateType() const {
	return EPigStates::LayingDown;
}
