// Fill out your copyright notice in the Description page of Project Settings.


#include "PigAnimInstance.h"
#include "../Pig/PigStateMachine/PigStateMachine.h"
#include "../Pig/Movement/MovementController.h"

void UPigAnimInstance::SetPigState(EPigStates pigState) {
	m_xPigState = pigState;
}


void UPigAnimInstance::OnStateFinished(EPigStates pigState) {
	OnEvent(pigState);
}


void UPigAnimInstance::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);
	auto stateMachine = GetStateMachine();
	stateMachine->Subscribe(this, EStateMachineEvent::StateChanged, [this](EPigStates oldState, EPigStates newState) {
		SetPigState(newState);
	});
	m_xPigState = stateMachine->GetCurrentStateType();
}


void UPigAnimInstance::DisengageAnimInstance() {
	GetStateMachine()->Unsibscribe(this);
}

void UPigAnimInstance::PreUpdateAnimation(float DeltaSeconds) {
	Super::PreUpdateAnimation(DeltaSeconds);

	if(!GetPig()) {
		m_fVelocity = 0;
		m_fRotation = 0;
		return;
	}

	m_fVelocity = GetMovementController()->GetVelocity();
	m_fRotation = GetMovementController()->GetRotation();
}
