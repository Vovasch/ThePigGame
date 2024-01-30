#include "PigSleepingState.h"

UPigSleepingState::UPigSleepingState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	SetStateType(EPigStates::Sleeping);
}