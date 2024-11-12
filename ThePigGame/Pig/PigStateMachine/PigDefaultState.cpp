#include "PigDefaultState.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskDispatcher/TaskDispatcher.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Base/BaseTask.h"
#include "ThePigGame/Pig/TasksInfrastructure/Tasks/Base/TaskEvent.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskType/TaskType.h"

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