#include "PigStateMachine.h"
#include "../../Pig/Pig.h"
#include "../PigAI/PigAIController.h"
#include "../../Animation/PigAnimInstance.h"

#include "PigDefaultState.h"
#include "PigEatingState.h"
#include "PigSleepingState.h"
#include "PigLayingDownState.h"
#include "PigStandingUpState.h"

DEFINE_LOG_CATEGORY_STATIC(StateMachineLog, Log, All)

UPigStateMachine::UPigStateMachine() {
	AddState(MakeShared<UPigDefaultState>(TArray{ EPigStates::Eating, EPigStates::LayingDown }));
	
	AddState(MakeShared<UPigEatingState>(TArray{ EPigStates::Default }));
	
	AddState(MakeShared<UPigLayingDownState>(TArray{ EPigStates::Sleeping }));
	AddState(MakeShared<UPigSleepingState>(TArray{ EPigStates::StandingUp }));
	AddState(MakeShared<UPigStandingUpState>(TArray{ EPigStates::Default }));
}

void UPigStateMachine::Init(APig* pigOwner) {
	ICachedPigDataUser::Init(pigOwner);


	for(auto& pigState : m_vAllStates) {
		pigState.Value->Init(GetPig());
	}

	m_pCurrentState = GetState(EPigStates::Default);
}

void UPigStateMachine::OnStateChanged(EPigStates oldState, EPigStates newState) {
	UE_LOG(StateMachineLog, Log, TEXT("State changed from %s to %s. %s"), *UEnum::GetValueAsString<EPigStates>(oldState), *UEnum::GetValueAsString<EPigStates>(newState), *GetPig()->GetName());
}

void UPigStateMachine::AfterInit() {
	m_pCurrentState->Start();
	OnEvent(EStateMachineEvent::StateChanged, EPigStates::Default, m_pCurrentState->StateType());
}
