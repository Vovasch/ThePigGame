#pragma once

#include "../../Utils/StateMachine/TStateBase.h"
#include "PigStates.h"


class UPigStandingUpState : public TStateBase<EPigStates> {

	public:
	UPigStandingUpState(TArray<EPigStates>&& transitions);

};