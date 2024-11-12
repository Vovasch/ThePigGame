#include "TaskBase.h"
#include "CoreMinimal.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskDispatcher/TaskDispatcher.h"

DEFINE_LOG_CATEGORY(TaskLog)

void UTaskBase::Start() {
	if(m_bInProgress) {
		UE_LOG(TaskLog, Fatal, TEXT("Task %s DIDNOT started. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());

		return;
	}
	UE_LOG(TaskLog, Log, TEXT("Task %s started. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::Start);
	m_bInProgress = true;
}

void UTaskBase::OnEnd() {
	m_bInProgress = false;
	UE_LOG(TaskLog, Log, TEXT("Task %s ended. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::End);
	m_pTaskData.Reset();
	GetTaskDispatcher()->OnEndTask(GetTaskType());
}

void UTaskBase::Complete() {
	if(!m_bInProgress) return;
	UE_LOG(TaskLog, Log, TEXT("Task %s completed. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::Success);
	OnEnd();
}

void UTaskBase::Fail() {
	if(!m_bInProgress) return;

	UE_LOG(TaskLog, Log, TEXT("Task %s failed. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
	OnEvent(ETaskEvent::Fail);
	OnEnd();
}

void UTaskBase::Tick(float delta) {

}

bool UTaskBase::IsInProgress() {
	return m_bInProgress;
}

void UTaskBase::SetTaskData(TStrongObjectPtr<const UTaskDataBase> taskData) {
	m_pTaskData = taskData;
}
