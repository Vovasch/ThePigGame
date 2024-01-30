#pragma once

#include "../../Utils/StateMachine/TStateBase.h"
#include "PigStates.h"

class UPigEatingState : public TStateBase<EPigStates> {

	public:
	UPigEatingState(TArray<EPigStates>&& transitions);

	public:
	virtual void Tick(float delta) override;

};