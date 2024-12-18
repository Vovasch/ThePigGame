#include "WalkingController.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PigAI/PigAIController.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"

AWalkingController::AWalkingController() {
	PrimaryActorTick.bCanEverTick = true;
}

void AWalkingController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	UpdateMaxSpeed();
}

float AWalkingController::GetLinearVelocityMagnitude() {
	return SpeedFromScale(GetPig()->GetVelocity().Length());
}

void AWalkingController::WalkTo(const FVector& loc) {
	BPWalkToLocation(loc);
}

void AWalkingController::InterruptWalking() {
	BPInterruptWalking();
}

void AWalkingController::InitWalkingData(const FPigWalkingData& initData) {
	m_vWalkingSpeeds[uint32(EMovementType::Sneak)] = initData.SneakSpeed;
	m_vWalkingSpeeds[uint32(EMovementType::SlowWalk)] = initData.SlowWalkSpeed;
	m_vWalkingSpeeds[uint32(EMovementType::Walk)] = initData.WalkSpeed;
	m_vWalkingSpeeds[uint32(EMovementType::Trot)] = initData.TrotSpeed;
	m_vWalkingSpeeds[uint32(EMovementType::Run)] = initData.RunSpeed;
	m_vWalkingSpeeds[uint32(EMovementType::Sprint)] = initData.SprintSpeed;
}

float AWalkingController::SpeedToScale(float baseSpeed) {
	return baseSpeed * GetPropertyController()->GetProperty<EPigPropertyType::Scale>()->GetCurrent();
}

float AWalkingController::SpeedFromScale(float speed) {
	return speed / GetPropertyController()->GetProperty<EPigPropertyType::Scale>()->GetCurrent();
}

void AWalkingController::SetWalkingType(EMovementType movementType) {
	m_xCurrentWalkingType = movementType;
}

float AWalkingController::GetWalkingSpeedFor(EMovementType movementType) {
	return m_vWalkingSpeeds[int32(movementType)];
}

void AWalkingController::UpdateMaxSpeed() {
	GetPig()->GetCharacterMovement()->MaxWalkSpeed = SpeedToScale(GetWalkingSpeedFor(m_xCurrentWalkingType));
}

void AWalkingController::OnWalkingCompleted() {
	OnEvent(EWalkingControllerEvent::WalkingCompleted);
}

void AWalkingController::OnWalkingFailed() {
	OnEvent(EWalkingControllerEvent::WalkingFailed);
}

AAIController* AWalkingController::GetAIControllerBP() {
	return GetAIController();
}