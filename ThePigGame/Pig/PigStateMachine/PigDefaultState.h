#pragma once

#include "../../Utils/StateMachine/TStateBase.h"
#include "PigStates.h"


class UPigDefaultState : public TStateBase<EPigStates> {

	public:
	UPigDefaultState(TArray<EPigStates>&& transitions);

	public:
	virtual void Init(APig* pig) override;

	public:
	virtual void Start() override;

	public:
	EPigStates StateType() const override;

};