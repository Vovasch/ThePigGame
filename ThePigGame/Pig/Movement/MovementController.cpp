#include "MovementController.h"
#include "../Pig.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMovementController::InitChildMovementSpeeds() {
	InitMovementSpeeds(false);
}

void UMovementController::InitAdultMovementSpeeds() {
	InitMovementSpeeds(true);
}

void UMovementController::InitMovementSpeeds(bool isAdult) {
	auto& initData = GetPig()->GetInitData();

	m_vMovementSpeeds[uint32(EMovementType::Sneak)] = isAdult ? initData.AdultSneakSpeed : initData.ChildSneakSpeed;
	m_vMovementSpeeds[uint32(EMovementType::SlowWalk)] = isAdult ? initData.AdultSlowWalkSpeed : initData.ChildSlowWalkSpeed;
	m_vMovementSpeeds[uint32(EMovementType::Walk)] = isAdult ? initData.AdultWalkSpeed : initData.ChildWalkSpeed;
	m_vMovementSpeeds[uint32(EMovementType::Trot)] = isAdult ? initData.AdultTrotSpeed : initData.ChildTrotSpeed;
	m_vMovementSpeeds[uint32(EMovementType::Run)] = isAdult ? initData.AdultRunSpeed : initData.ChildRunSpeed;
	m_vMovementSpeeds[uint32(EMovementType::Sprint)] = isAdult ? initData.AdultSprintSpeed : initData.ChildSprintSpeed;
}

void UMovementController::SetMovementType(EMovementType movementType) {
	m_xCurrentMovementType = movementType;
}

void UMovementController::UpdateMaxSpeed() {
	GetPig()->GetCharacterMovement()->MaxWalkSpeed = m_vMovementSpeeds[uint32(m_xCurrentMovementType)]* GetPig()->GetCurrentScale();
}