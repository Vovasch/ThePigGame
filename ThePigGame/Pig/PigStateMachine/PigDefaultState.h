#pragma once

#include "../../Utils/StateMachine/TStateBase.h"
#include "PigStates.h"


class UPigDefaultState : public TStateBase<EPigStates> {

	public:
	UPigDefaultState(TArray<EPigStates>&& transitions);

};