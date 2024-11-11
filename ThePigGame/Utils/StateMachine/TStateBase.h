#pragma once

#include "StateEvent.h"
#include "../EventHandler/TEventHandler.h"
#include "../PigDataUser/IPigDataUser.h"

// todo there is not need for template. better remake it to be UObject derived class. and then remake creation of states in UStateMachine
template<typename EnumType>
class TStateBase : public TEventHandler<EStateEvent>, public ICachedPigDataUser {
	public:
	TStateBase(TArray<EnumType>&& transitions) {
		m_vTransitions = transitions;
	}

	public:
	virtual ~TStateBase()=default;

	public:
	EnumType StateType() const { return m_xStateType; }

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

	protected:
	// todo remove this stupid method and make pure virtual getter
	void SetStateType(EnumType enumVal) {
		m_xStateType = enumVal;
	}

	protected:
	EnumType m_xStateType;

	protected:
	TArray<EnumType> m_vTransitions;
};