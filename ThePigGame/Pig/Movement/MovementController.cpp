#include "MovementController.h"
#include "../Pig.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Animation/PigAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(MovementControllerLog, Log, All)

const int s_iRightRotationMul = 1;
const int s_iLeftRotationMul = -1;

const int s_iRotationToRight = 180;
const int s_iRotationToLeft = -180;

const EMovementType s_xRotationMovementType = EMovementType::Walk;

float UMovementController::GetVelocity() {
	return m_fVelocity;
}

float UMovementController::GetRotation() {
	return m_fRotation;
}

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

	const auto& rotationData = isAdult ? initData.AdultRotationData : initData.ChildRotationData;

	m_xRotationData.m_dDistance = FVector(rotationData.WalkDeltaX, rotationData.WalkDeltaY, 0).Length();
	m_xRotationData.m_fDegrees = rotationData.DegreesInTime;
	m_xRotationData.m_fTime = rotationData.TimeTaken;

	if(m_bIsCurrentlyRotating) RecalcRotationData();
}

void UMovementController::SetMovementType(EMovementType movementType) {
	m_xCurrentMovementType = movementType;
}

void UMovementController::EndPeriod() {
	m_fTimePerPeriodCounter = 0.f;
}

void UMovementController::Tick(float deltaTime) {
	UpdateMaxSpeed();

	if(m_bIsCurrentlyRotating) {

		m_fRotationTimeCounter += deltaTime;
		m_fTimePerPeriodCounter += deltaTime;

		if(m_fTimePerPeriodCounter>=m_xRotationData.m_fTime) {
			EndPeriod();
		}

		if(m_fRotationTimeCounter >= m_fTimeToRotate) {
			EndRotating();
		}

		auto locationDt = SpeedToScale(m_xRotationData.m_dDistance) * deltaTime / m_xRotationData.m_fTime;

		auto forwardVector = GetPig()->GetActorForwardVector();
		FVector newLocation = GetPig()->GetActorLocation() + forwardVector * locationDt;
		GetPig()->SetActorLocation(newLocation);
			
		auto yawDt = m_iRotationMul * m_xRotationData.m_fDegrees * deltaTime / m_xRotationData.m_fTime;
		auto deltaRotator = FRotator(0, yawDt, 0);
		GetPig()->AddActorWorldRotation(deltaRotator);		

	} else {
		m_fVelocity = SpeedFromScale(GetPig()->GetVelocity().Length());
		m_fRotation = GetPig()->GetPigAnimInstance()->CalculateDirection(GetPig()->GetVelocity(), GetPig()->GetActorRotation());
	}
}

void UMovementController::UpdateMaxSpeed() {
	GetPig()->GetCharacterMovement()->MaxWalkSpeed = SpeedToScale(m_vMovementSpeeds[uint32(m_xCurrentMovementType)]);
}

void UMovementController::RotateTo(const FVector& location) {
	m_xRotatingTo = location;
	m_bIsCurrentlyRotating = true;
	RecalcRotationData();
}

void UMovementController::RecalcRotationData() {
	EndPeriod();

	m_fRotationTimeCounter = 0;

	auto pigRotation = GetPig()->GetActorRotation().Yaw + 180;
	auto newRotator = UKismetMathLibrary::FindLookAtRotation(GetPig()->GetActorLocation(), m_xRotatingTo).Yaw + 180;

	auto degreesToRotate = newRotator - pigRotation;

	//degreesToRotate = -130; // for debug

	if(degreesToRotate > 180) {
		degreesToRotate -= 360;
	} else if(degreesToRotate < -180) {
		degreesToRotate += 360;
	}

	auto timesToRotate = FMath::Abs(degreesToRotate) / m_xRotationData.m_fDegrees;

	m_fTimeToRotate = timesToRotate * m_xRotationData.m_fTime;

	auto isRotationToRight = degreesToRotate > 0;

	m_iRotationMul = isRotationToRight ? s_iRightRotationMul : s_iLeftRotationMul;

	m_fVelocity = m_vMovementSpeeds[uint32(s_xRotationMovementType)];
	m_fRotation = isRotationToRight ? s_iRotationToRight : s_iRotationToLeft;
}

void UMovementController::EndRotating() {
	m_bIsCurrentlyRotating = false;
	OnEvent(EMovementControllerEvent::RotationFinished);
	//RotateTo(FVector::ZeroVector); // for debug
}

float UMovementController::SpeedToScale(float baseSpeed) {
	return baseSpeed * GetPig()->GetCurrentScale();
}

float UMovementController::SpeedFromScale(float speed) {
	return speed / GetPig()->GetCurrentScale();
}
