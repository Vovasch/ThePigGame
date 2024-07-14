#include "InterruptibleTask.h"
#include "../../Pig.h"


void UInterruptibleTask::Interrupt() {
	if(!IsInProgress()) return;
	OnEnd();
	OnEvent(ETaskEvent::Interrupt);
	UE_LOG(TaskLog, Log, TEXT("Task %s interrupted. %s"), *UEnum::GetDisplayValueAsText(m_xTaskType).ToString(), *GetPig()->GetName());
}