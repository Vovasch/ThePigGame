#pragma once

#include "StateEvent.h"
#include "../EventHandler/TEventHandler.h"
#include "../PigDataUser/IPigDataUser.h"

template<typename EnumType>
class TStateBase : public TEventHandler<EStateEvent>, public ICachedPigDataUser {
	public:
	TStateBase(TArray<EnumType>&& transitions) {
		m_vTransitions = transitions;
	}

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
	void SetStateType(EnumType enumVal) {
		m_xStateType = enumVal;
	}

	protected:
	EnumType m_xStateType;

	protected:
	TArray<EnumType> m_vTransitions;
};