#include "BaseTask.h"
#include "CoreMinimal.h"
#include "../../Pig.h"
#include "TaskDispatcher.h"

DEFINE_LOG_CATEGORY(TaskLog)

void UBaseTask::Start() {
	if(m_bInProgress) {
		UE_LOG(TaskLog, Log, TEXT("Task %s DIDNOT started. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());

		return;
	}
	UE_LOG(TaskLog, Log, TEXT("Task %s started. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
	OnEvent(ETaskEvent::Start, m_xTaskType);
	m_bInProgress = true;
}

void UBaseTask::OnEnd() {
	m_bInProgress = false;
	UE_LOG(TaskLog, Log, TEXT("Task %s ended. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
	OnEvent(ETaskEvent::End, m_xTaskType);
}

void UBaseTask::Complete() {
	if(!m_bInProgress) return;
	UE_LOG(TaskLog, Log, TEXT("Task %s completed. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
	OnEnd();
	OnEvent(ETaskEvent::Success, m_xTaskType);
}

void UBaseTask::Fail() {
	if(!m_bInProgress) return;
	UE_LOG(TaskLog, Log, TEXT("Task %s failed. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
	OnEnd();
	OnEvent(ETaskEvent::Fail, m_xTaskType);
}

void UBaseTask::Tick(float delta) {

}

ETaskType UBaseTask::GetTaskType() {
	return m_xTaskType;
}

bool UBaseTask::IsInProgress() {
	return m_bInProgress;
}