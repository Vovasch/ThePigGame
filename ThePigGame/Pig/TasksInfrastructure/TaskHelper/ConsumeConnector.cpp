#include "ConsumeConnector.h"

DEFINE_LOG_CATEGORY_STATIC(ConsumeConnectorLog, Log, All)

ETaskType ConsumeConnector::TaskBySource(EConsumeSourceType sourceType) {
	return GetConnections().m_xSourceToTask[sourceType];
}

EConsumeSourceType ConsumeConnector::SourceByTask(ETaskType taskType) {
	return GetConnections().m_xTaskToSource[taskType];
}

const ConsumeConnector& ConsumeConnector::GetConnections() {
	static const auto connnections = GenerateConnections();
	return connnections;
}

ConsumeConnector ConsumeConnector::GenerateConnections() {
	// todo check if this is called only once
	ConsumeConnector result;

	result.AddConnection(ETaskType::GoToEatingSpot, EConsumeSourceType::Eating, result);
	result.AddConnection(ETaskType::GoToDrinkingSpot, EConsumeSourceType::Drinking, result);

	const auto requiredNum = uint32(EConsumeSourceType::Size);
	if(result.m_xSourceToTask.Num()!=requiredNum || result.m_xTaskToSource.Num()!=requiredNum) {
		UE_LOG(ConsumeConnectorLog, Fatal, TEXT("Number of connections is not the same as EConsumeSourceType::Size, which is required"));
	}

	return result;
}

void ConsumeConnector::AddConnection(ETaskType taskType, EConsumeSourceType sourceType, ConsumeConnector& connector) {
	connector.m_xSourceToTask.Add(sourceType, taskType);
	connector.m_xTaskToSource.Add(taskType, sourceType);
}
