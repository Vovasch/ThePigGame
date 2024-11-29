#pragma once

#include "StateEvent.h"
#include "../EventHandler/TEventHandler.h"
#include "../PigDataUser/IPigDataUser.h"

// todo SaveSystemTask there is not need for template. better remake it to be UObject derived class. and then remake creation of states in UStateMachine
template<typename EnumType>
class TStateBase : public TEventHandler<EStateEvent>, public ICachedPigDataUser {
	public:
	TStateBase(TArray<EnumType>&& transitions) {
		m_vTransitions = transitions;
	}

	public:
	virtual ~TStateBase()=default;

	public:
	virtual EnumType StateType() const=0;

	public:
	bool CanTransitTo(EnumType enumVal) {
		return m_vTransitions.Find(enumVal)!=INDEX_NONE;
	}

	public:
	virtual void Tick(float delta) {

	}

	virtual void Start() {
		OnEvent(EStateEvent::Start);
	}

	virtual void End() {
		OnEvent(EStateEvent::End);
	}

	private:
	TArray<EnumType> m_vTransitions;
};