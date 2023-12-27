#pragma once

#include "../../Utils/StateMachine/TStateBase.h"
#include "PigStates.h"

class UPigSleepingState : public TStateBase<EPigStates> {

	public:
	UPigSleepingState(TArray<EPigStates>&& transitions);

};