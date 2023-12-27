#include "PigEatingState.h"

UPigEatingState::UPigEatingState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	Init(EPigStates::Eating);
}