#include "PigStandingUpState.h"

UPigStandingUpState::UPigStandingUpState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	Init(EPigStates::StandingUp);
}