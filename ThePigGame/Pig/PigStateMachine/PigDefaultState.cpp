#include "PigDefaultState.h"

UPigDefaultState::UPigDefaultState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	Init(EPigStates::Default);
}