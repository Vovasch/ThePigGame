#include "SleepingController.h"

#include "ThePigGame/Levels/MainLevel/GameModeEvent.h"
#include "ThePigGame/Levels/MainLevel/MainScreenGameMode.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PigStateMachine/PigStateMachine.h"
#include "ThePigGame/Pig/Tasks/Base/TaskDispatcher.h"


const Energy* USleepingController::GetEnergy() {
	return &m_xEnergy;
}

void USleepingController::Tick(float delta) {
	Super::Tick(delta);
	CheckEnergyLevel();
}

void USleepingController::InitProperties() {
	m_xEnergy.Init(this);
	m_xEnergy.GetMinMaxType().SetMinMax(GetInitData()->MinEnergy, GetInitData()->MaxEnergy);
	m_xEnergy.SetDelta(GetInitData()->LoosingEnergyDelta);
	m_xEnergy.GetCurrentModifycationType().Set(50);


	auto gameMode = Cast<AMainScreenGameMode>(GetPig()->GetWorld()->GetAuthGameMode());

	gameMode->Subscribe(this, EGameModeEvent::NightStarted, [this]() {
		GoToSleep();
	});

	gameMode->Subscribe(this, EGameModeEvent::MorningStarted, [this]() {
		WakeUp();
	});
}

void USleepingController::GoToSleep() {
	// TODO: make changing delta on start of sleep

	auto taskDispatcher = GetTaskDispatcher();

	taskDispatcher->GetTaskByType(ETaskType::GoToSleep)->Subscribe(this, ETaskEvent::Success, [this]() {
		m_xEnergy.SetDelta(GetInitData()->GainignEnergyDelta);
		GetStateMachine()->TryChangeState(EPigStates::LayingDown);
	});

	// todo: subscribe on failed, but at first implement it in task

	GetTaskDispatcher()->AddTask(ETaskType::GoToSleep);
}

void USleepingController::WakeUp() {
	m_xEnergy.SetDelta(GetInitData()->LoosingEnergyDelta);
	GetStateMachine()->TryChangeState(EPigStates::StandingUp);
}

void USleepingController::CheckEnergyLevel() {
	auto energyLevel = m_xEnergy.GetCurrent();
	
	if(energyLevel <= GetInitData()->EnergyLevelToWantToSleepLevel) {
		// TODO: integrate into sleeping system
		//AddTask(ETaskType::GoToSleep);
	} else if(energyLevel >= GetInitData()->EnergyLevelSleepedEnought) {
		// TODO: integrate into sleeping system
		//m_pStateMachine->TryChangeState(EPigStates::StandingUp);
	}
}