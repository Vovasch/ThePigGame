#include "TBaseTask.h"

void TBaseTask::Start() {
	if(m_bInProgress) return;
	OnEvent(ETaskEvent::Start, m_xTaskType);
	m_bInProgress = true;
}

void TBaseTask::OnEnd() {
	m_bInProgress = false;
	OnEvent(ETaskEvent::End, m_xTaskType);
}

void TBaseTask::Complete() {
	if(!m_bInProgress) return;
	OnEnd();
	OnEvent(ETaskEvent::Success, m_xTaskType);
}

void TBaseTask::Fail() {
	if(!m_bInProgress) return;
	OnEnd();
	OnEvent(ETaskEvent::Fail, m_xTaskType);
}

void TBaseTask::Tick(float delta) {

}

ETaskType TBaseTask::GetTaskType() {
	return m_xTaskType;
}