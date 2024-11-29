#include "PigStandingUpState.h"

UPigStandingUpState::UPigStandingUpState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {}

EPigStates UPigStandingUpState::StateType() const {
	return EPigStates::StandingUp;
}
