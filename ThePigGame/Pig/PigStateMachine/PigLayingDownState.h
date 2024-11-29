#pragma once

#include "../../Utils/StateMachine/TStateBase.h"
#include "PigStates.h"

class UPigLayingDownState : public TStateBase<EPigStates> {

	public:
	UPigLayingDownState(TArray<EPigStates>&& transitions);

	public:
	EPigStates StateType() const override;
};