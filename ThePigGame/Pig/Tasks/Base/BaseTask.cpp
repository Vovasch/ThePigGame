#include "BaseTask.h"
#include "CoreMinimal.h"
#include "../../Pig.h"
#include "ThePigGame/Pig/Tasks/TaskDispatcher/TaskDispatcher.h"

DEFINE_LOG_CATEGORY(TaskLog)

void UBaseTask::Start() {
	if(m_bInProgress) {
		UE_LOG(TaskLog, Fatal, TEXT("Task %s DIDNOT started. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());

		return;
	}
	UE_LOG(TaskLog, Log, TEXT("Task %s started. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::Start);
	m_bInProgress = true;
}

void UBaseTask::OnEnd() {
	m_bInProgress = false;
	UE_LOG(TaskLog, Log, TEXT("Task %s ended. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::End);
	m_pTaskData.Reset();
	GetTaskDispatcher()->OnEndTask(GetTaskType());
}

void UBaseTask::Complete() {
	if(!m_bInProgress) return;
	UE_LOG(TaskLog, Log, TEXT("Task %s completed. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::Success);
	OnEnd();
}

void UBaseTask::Fail() {
	if(!m_bInProgress) return;

	UE_LOG(TaskLog, Log, TEXT("Task %s failed. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::Fail);
	OnEnd();
}

void UBaseTask::Tick(float delta) {

}

bool UBaseTask::IsInProgress() {
	return m_bInProgress;
}

void UBaseTask::SetTaskData(TStrongObjectPtr<const UTaskDataBase> taskData) {
	m_pTaskData = taskData;
}
