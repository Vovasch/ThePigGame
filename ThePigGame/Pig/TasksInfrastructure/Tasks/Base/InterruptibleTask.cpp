#include "InterruptibleTask.h"

void UInterruptibleTask::Interrupt() {
	if(!IsInProgress()) return;
	OnEnd();
	OnEvent(ETaskEvent::Interrupt);
	UE_LOG(TaskLog, Log, TEXT("Task %s interrupted. %s"), *UEnum::GetDisplayValueAsText(GetTaskType()).ToString(), *GetPig()->GetName());
}