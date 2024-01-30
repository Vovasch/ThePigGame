#include "PigLayingDownState.h"

UPigLayingDownState::UPigLayingDownState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	SetStateType(EPigStates::LayingDown);
}