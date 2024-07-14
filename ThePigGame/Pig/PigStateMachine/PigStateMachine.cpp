#include "PigStateMachine.h"
#include "../../Pig/Pig.h"
#include "../PigAI/PigAIController.h"
#include "PigDefaultState.h"
#include "PigEatingState.h"
#include "PigSleepingState.h"
#include "PigLayingDownState.h"
#include "PigStandingUpState.h"
#include "ThePigGame/Pig/AnimInstance/AnimInstanceController.h"
#include "ThePigGame/Pig/AnimInstance/PigAnimInstance.h"

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
	UE_LOG(StateMachineLog, Log, TEXT("State changed from %s to %s. %s"), *UEnum::GetDisplayValueAsText(oldState).ToString(), *UEnum::GetDisplayValueAsText(newState).ToString(), *GetPig()->GetName());
}

void UPigStateMachine::AfterInit() {
	m_pCurrentState->Start();
	SubscribeOnAnimInstanceController();
	SubscribeOnAnimInstance();

	OnEvent(EStateMachineEvent::StateChanged, EPigStates::Default, m_pCurrentState->StateType());
}

void UPigStateMachine::SubscribeOnAnimInstanceController() {
	GetAnimInstanceController()->Subscribe(this, EAnimInstanceControllerEvent::OnBeforeDisengaged, [this]() {
		UnsubscribeFromAnimInstance();
	});

	GetAnimInstanceController()->Subscribe(this, EAnimInstanceControllerEvent::OnAfterEnabled, [this]() {
		SubscribeOnAnimInstance();
	});
}

void UPigStateMachine::SubscribeOnAnimInstance() {
	auto animInstace = GetPig()->GetPigAnimInstance();
	animInstace->Subscribe(this, EPigStates::LayingDown, [this]() {
		TryChangeState(EPigStates::Sleeping);
	});

	animInstace->Subscribe(this, EPigStates::StandingUp, [this]() {
		TryChangeState(EPigStates::Default);
	});
}

void UPigStateMachine::UnsubscribeFromAnimInstance() {
	GetPig()->GetPigAnimInstance()->Unsubscribe(this);
}
