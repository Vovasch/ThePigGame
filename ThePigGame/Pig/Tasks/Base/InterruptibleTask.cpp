#include "InterruptibleTask.h"
#include "../../Pig.h"


void UInterruptibleTask::Interrupt() {
	if(!IsInProgress()) return;
	OnEnd();
	OnEvent(ETaskEvent::Interrupt, m_xTaskType);
	UE_LOG(TaskLog, Log, TEXT("Task %s interrupted. %s"), *UEnum::GetValueAsString<ETaskType>(m_xTaskType), *GetPig()->GetName());
}