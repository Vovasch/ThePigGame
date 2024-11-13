#include "PigInitData.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"

UPigInitData::UPigInitData() {
	ChildRotationData.WalkDeltaX = 13.84f;
	ChildRotationData.WalkDeltaY = 23.98f;
	ChildRotationData.TimeTaken = 0.93f;
	ChildRotationData.DegreesInTime = 60;

	AdultRotationData.WalkDeltaX = 32.03f;
	AdultRotationData.WalkDeltaY = 68.69f;
	AdultRotationData.TimeTaken = 1.1f;
	AdultRotationData.DegreesInTime = 50.f;


	ChildWalkingData.SneakSpeed = 7.43f;
	ChildWalkingData.SlowWalkSpeed = 22.44f;
	ChildWalkingData.WalkSpeed = 31.07f;
	ChildWalkingData.TrotSpeed = 133.6f;
	ChildWalkingData.RunSpeed = 225.f;
	ChildWalkingData.SprintSpeed = 418.125f;

	AdultWalkingData.SneakSpeed = 20.f;
	AdultWalkingData.SlowWalkSpeed = 51.9f;
	AdultWalkingData.WalkSpeed = 72.04f;
	AdultWalkingData.TrotSpeed = 237.61f;
	AdultWalkingData.RunSpeed = 514.05f;
	AdultWalkingData.SprintSpeed = 820.88f;

	// todo check if changed values in engine's UI would be reverted if new EConsumeSourceTyped added
	for(uint32 i = 0; i<uint32(EConsumeSourceType::Size); ++i) {
		ConsumingData.Add(EConsumeSourceType(i), FConsumingData{});
	}
}