#include "TGoToSleepTask.h"
#include "../../Pig/Pig.h"
#include "../../Farm/Farm.h"
#include "../../Farm/SleepingArea/SleepingArea.h"
#include "Math/Box.h"

TGoToSleepTask::TGoToSleepTask() {
	m_xTaskType = ETaskType::GoToSleep;
}

void TGoToSleepTask::Start() {

	auto pig = GetPig();
	auto sleepingArea = pig->GetOwnerFarm()->GetSleepingArea();

	auto box = FBox();
	box = box.TransformBy(sleepingArea->GetComponentTransform());

	auto isPigInsideSleepingArea = box.IsInside(pig->GetActorLocation());

	

	if(isPigInsideSleepingArea) {
		GoToSleepingPlace();
	} else {
		
	}

}

void TGoToSleepTask::GoToSleepingPlace() {
	auto sleepingLocation = FVector::ZeroVector;
}