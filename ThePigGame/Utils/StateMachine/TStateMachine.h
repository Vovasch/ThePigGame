#pragma once

#include "CoreMinimal.h"
#include "TStateBase.h"
#include "../EventHandler/TEventHandler.h"
#include "StateMachineEvent.h"


template <typename EnumType>
class TStateMachine : public TEventHandler<EStateMachineEvent, EnumType, EnumType> {
	
	protected:
	using TStateType = TStateBase<EnumType>;

	private:
	using ParentType = TEventHandler<EStateMachineEvent, EnumType, EnumType>;

	protected:
	using ParentType::OnEvent;

	public:
	TStateMachine()=default;
	virtual ~TStateMachine()=default;

	public:
	EnumType GetCurrentStateType() { return m_pCurrentState->StateType(); }

	public:
	TSharedPtr<TStateType> GetState(EnumType enumVal) {
		return m_vAllStates[enumVal];
	}

	public:
	virtual bool TryChangeState(EnumType newState) {
		if(!m_pCurrentState->CanTransitTo(newState)) return false;

		auto oldState = m_pCurrentState->StateType();

		m_pCurrentState->End();

		m_pCurrentState = m_vAllStates[newState];

		m_pCurrentState->Start();

		OnEvent(EStateMachineEvent::StateChanged, oldState, newState);

		OnStateChanged(oldState, newState);

		return true;
	}

	public:
	void Tick(float deltaTime) {
		m_pCurrentState->Tick(deltaTime);
	}

	protected:
	virtual void OnStateChanged(EnumType oldState, EnumType newState) = 0;

	protected:
	void AddState(TSharedPtr<TStateType> state) {
		m_vAllStates.Add(state->StateType(), state);
	}

	protected:
	TSharedPtr<TStateType> m_pCurrentState = nullptr;

	protected:
	TMap<EnumType, TSharedPtr<TStateType>> m_vAllStates;
};