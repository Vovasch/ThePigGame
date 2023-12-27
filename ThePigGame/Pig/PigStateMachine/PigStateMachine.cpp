#include "PigStateMachine.h"
#include "../../Pig/Pig.h"
#include "../PigAI/PigAIController.h"
#include "../../Animation/PigAnimInstance.h"

#include "PigDefaultState.h"

#include "PigEatingState.h"
#include "PigSleepingState.h"
#include "PigLayingDownState.h"
#include "PigStandingUpState.h"


UPigStateMachine::UPigStateMachine() {

	AddState(MakeShared<UPigDefaultState>(TArray{ EPigStates::Eating, EPigStates::LayingDown }));
	
	AddState(MakeShared<UPigEatingState>(TArray{ EPigStates::Default }));
	
	AddState(MakeShared<UPigLayingDownState>(TArray{ EPigStates::Sleeping }));
	AddState(MakeShared<UPigSleepingState>(TArray{ EPigStates::StandingUp }));
	AddState(MakeShared<UPigStandingUpState>(TArray{ EPigStates::Default }));

	m_pCurrentState = GetState(EPigStates::Default);

}

void UPigStateMachine::SetPigOwner(APig* pigOwner) {
	m_pPigOwner = pigOwner;
	OnEvent(EStateMachineEvent::StateChanged, EPigStates::Default, m_pCurrentState->StateType());	
}