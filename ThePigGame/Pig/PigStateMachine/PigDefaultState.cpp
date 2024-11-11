#include "PigDefaultState.h"
#include "../../Pig/Tasks/Derived/GoToRandomLocationTask.h"
#include "ThePigGame/Pig/Tasks/TaskDispatcher/TaskDispatcher.h"

using ParentTaskType = TStateBase<EPigStates>;

UPigDefaultState::UPigDefaultState(TArray<EPigStates>&& transitions) : TStateBase<EPigStates>(std::move(transitions)) {
	SetStateType(EPigStates::Default);
}

void UPigDefaultState::Init(APig* pig) {
	ParentTaskType::Init(pig);

	GetTaskDispatcher()->GetTaskByType(ETaskType::GoToRandomLocation)->Subscribe(this, ETaskEvent::End, [this]() {
		GetTaskDispatcher()->AddTask(ETaskType::GoToRandomLocation);
	});
}

void UPigDefaultState::Start() {
	ParentTaskType::Start();
	GetTaskDispatcher()->AddTask(ETaskType::GoToRandomLocation);
}