#include "TBaseTask.h"
#include "CoreMinimal.h"
#include "../Pig.h"

DEFINE_LOG_CATEGORY_STATIC(TaskLog, Log, All)

void TBaseTask::Start() {
	if(m_bInProgress) return;
	OnEvent(ETaskEvent::Start, m_xTaskType);
	m_bInProgress = true;
	UE_LOG(TaskLog, Log, TEXT("Task %s started. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
}

void TBaseTask::OnEnd() {
	m_bInProgress = false;
	OnEvent(ETaskEvent::End, m_xTaskType);
}

void TBaseTask::Complete() {
	if(!m_bInProgress) return;
	OnEnd();
	OnEvent(ETaskEvent::Success, m_xTaskType);
	UE_LOG(TaskLog, Log, TEXT("Task %s completed. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
}

void TBaseTask::Fail() {
	if(!m_bInProgress) return;
	OnEnd();
	OnEvent(ETaskEvent::Fail, m_xTaskType);
	UE_LOG(TaskLog, Log, TEXT("Task %s failed. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
}

void TBaseTask::Tick(float delta) {

}

ETaskType TBaseTask::GetTaskType() {
	return m_xTaskType;
}