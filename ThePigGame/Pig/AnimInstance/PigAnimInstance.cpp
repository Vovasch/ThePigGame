#include <ThePigGame/Pig/AnimInstance/PigAnimInstance.h>
#include "ThePigGame/Farm/Components/ConsumeSpotComponent/ConsumeSpotComponent.h"
#include "ThePigGame/Pig/Movement/MovementController.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/ConsumingController/ConsumingController.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "ThePigGame/Utils/StateMachine/StateMachineEvent.h"

DEFINE_LOG_CATEGORY_STATIC(PigAnimInstanceLog, Log, All)

void UPigAnimInstance::SetPigState(EPigStates pigState) {
	m_xPigState = pigState;
}

void UPigAnimInstance::OnStateFinished(EPigStates pigState) {
	OnEvent(pigState);
}

EConsumeSourceType UPigAnimInstance::GetCurrentConsumeSpotType() {
	// todo hide this functionality and error inside of Consuming Controller by create a GetOccupiedSpotTypeChecked there
	auto spot = GetPropertyController()->GetConsumingController()->GetOccupiedSpot();
	if(!spot.IsValid()) {
		UE_LOG(PigAnimInstanceLog, Fatal, TEXT("Pig is not in eating state"));
	}
	return spot->GetSpotType();
}

void UPigAnimInstance::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);
	auto stateMachine = GetStateMachine();
	stateMachine->Subscribe(this, EStateMachineEvent::StateChanged, [this](EPigStates oldState, EPigStates newState) {
		SetPigState(newState);
	});
	m_xPigState = stateMachine->GetCurrentStateType();
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
