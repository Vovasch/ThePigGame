#include "GoToConsumeSpotTask.h"
#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Pig/Movement/MovementControllerEvent.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"
#include "ThePigGame/Farm/Components/ConsumeSpotComponent/ConsumeSpotComponent.h"
#include "ThePigGame/Farm/Controllers/ConsumeSpotsController/ConsumeSpotsController.h"
#include "ThePigGame/Pig/Movement/MovementController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/ConsumingController/ConsumingController.h"
#include "ThePigGame/Pig/TasksInfrastructure/TaskHelper/ConsumeConnector.h"

DEFINE_LOG_CATEGORY_STATIC(GoToConsumeSpotTaskLog, Log, All)

void UGoToConsumeSpotTask::Start() {
	UTaskBase::Start();
	TryGoToSpot();
}

void UGoToConsumeSpotTask::OnEnd() {
	UnsubscribeAll();
	UTaskBase::OnEnd();
}

void UGoToConsumeSpotTask::Tick(float delta) {
	// while going to spot another pig could occupy our eating spot,
	// so we try to find new one
	if(!m_pTargetConsumeSpot.IsValid() || !m_pTargetConsumeSpot->IsAvailable()) {
		TryGoToSpot();
	}
}

void UGoToConsumeSpotTask::TryGoToSpot() {
	UnsubscribeAll();

	auto consumeSourceType = GetSourceType();
	m_pTargetConsumeSpot = GetFarm()->GetConsumeSpotsController()->GetNearestFreeSpot(GetPig()->GetActorLocation(), consumeSourceType);

	if(!m_pTargetConsumeSpot.IsValid()) {
		Fail();
		return;
	}

	auto movementController = GetMovementController();
	movementController->Subscribe(this, EMovementControllerEvent::MovementCompleted, [this]() {
		OnMovementCompleted();
	});

	movementController->Subscribe(this, EMovementControllerEvent::MovementFailed, [this]() {
		Fail();
	});

	movementController->MoveTo(m_pTargetConsumeSpot->GetComponentLocation());
}

void UGoToConsumeSpotTask::OnMovementCompleted() {
	if(m_pTargetConsumeSpot.IsValid() && m_pTargetConsumeSpot->IsAvailable()) {
		GetPig()->GetPropertyController()->GetSubController<ESubControllerType::Consuming>()->StartConsuming(m_pTargetConsumeSpot);
		this->Complete();
	} else {
		TryGoToSpot();
	}
}

void UGoToConsumeSpotTask::UnsubscribeAll() {
	GetMovementController()->Unsubscribe(this);
}

EConsumeSourceType UGoToConsumeSpotTask::GetSourceType() {
	return ConsumeConnector::SourceByTask(GetTaskType());
}
