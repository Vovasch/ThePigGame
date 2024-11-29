#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskType/TaskType.h"

class ConsumeConnector {

	private:
	ConsumeConnector()=default;

	public:
	static ETaskType TaskBySource(EConsumeSourceType sourceType);
	static EConsumeSourceType SourceByTask(ETaskType taskType);

	private:
	static const ConsumeConnector& GetConnections();
	static ConsumeConnector GenerateConnections();
	static void AddConnection(ETaskType taskType, EConsumeSourceType sourceType, ConsumeConnector& connector);

	private:
	TMap<EConsumeSourceType, ETaskType> m_xSourceToTask;
	TMap<ETaskType, EConsumeSourceType> m_xTaskToSource;

};
