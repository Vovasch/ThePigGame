#pragma once

#include "../../Utils/StateMachine/TStateBase.h"
#include "PigStates.h"

class UPigConsumingState : public TStateBase<EPigStates> {

	public:
	UPigConsumingState(TArray<EPigStates>&& transitions);

	EPigStates StateType() const override;
};