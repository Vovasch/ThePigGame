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

void UPigStateMachine::Init(APig* pigOwner) {
	ICachedPigDataUser::Init(pigOwner);

	AddState(MakeShared<UPigDefaultState>(TArray{ EPigStates::Eating, EPigStates::LayingDown }));
	
	AddState(MakeShared<UPigEatingState>(TArray{ EPigStates::Default }));
	
	AddState(MakeShared<UPigLayingDownState>(TArray{ EPigStates::Sleeping }));
	AddState(MakeShared<UPigSleepingState>(TArray{ EPigStates::StandingUp }));
	AddState(MakeShared<UPigStandingUpState>(TArray{ EPigStates::Default }));


	for(auto& pigState : m_vAllStates) {
		pigState.Value->Init(GetPig());
	}

	m_pCurrentState = GetState(EPigStates::Default);
	OnEvent(EStateMachineEvent::StateChanged, EPigStates::Default, m_pCurrentState->StateType());	
}

bool UPigStateMachine::TryChangeState(EPigStates newState) {
	auto prevStateType = m_pCurrentState->StateType();
	auto res = TStateMachine<EPigStates>::TryChangeState(newState);
	UE_LOG(StateMachineLog, Log, TEXT("State changed from %s to %s. %s"), *UEnum::GetValueAsString<EPigStates>(prevStateType), *UEnum::GetValueAsString<EPigStates>(newState), *GetPig()->GetName());
	return res;
}