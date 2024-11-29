#include "PigConsumingState.h"

UPigConsumingState::UPigConsumingState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {}

EPigStates UPigConsumingState::StateType() const {
	return EPigStates::Consuming;
}