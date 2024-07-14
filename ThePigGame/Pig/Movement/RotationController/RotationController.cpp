#include "RotationController.h"

#include "Kismet/KismetMathLibrary.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/AnimInstance/PigAnimInstance.h"
#include "ThePigGame/Pig/Movement/MovementType.h"
#include "ThePigGame/Pig/Movement/WalkingController/WalkingController.h"

const EMovementType s_xRotationMovementType = EMovementType::Walk;

const int s_iRightRotationMul = 1;
const int s_iLeftRotationMul = -1;

const int s_iRotationToRight = 180;
const int s_iRotationToLeft = -180;

void URotationController::RotateTo(const FVector& vec) {

	EndRotating();
	/*m_xRotatingTo = vec;
	m_bIsCurrentlyRotating = true;
	RecalcRotationData();*/
}

void URotationController::Tick(float deltaTime) {
	if(!m_bIsCurrentlyRotating) return;

	m_fRotationTimeCounter += deltaTime;
	m_fTimePerPeriodCounter += deltaTime;

	if(m_fTimePerPeriodCounter >= m_xRotationData.m_fTime) {
		EndPeriod();
	}

	if(m_fRotationTimeCounter >= m_fTimeToRotate) {
		TryEndRotating();
	}

	auto locationDt = m_pWalkingController->SpeedToScale(m_xRotationData.m_dDistance) * deltaTime / m_xRotationData.m_fTime;

	auto forwardVector = GetPig()->GetActorForwardVector();
	FVector newLocation = GetPig()->GetActorLocation() + forwardVector * locationDt;
	GetPig()->SetActorLocation(newLocation);

	auto yawDt = m_iRotationMul * m_xRotationData.m_fDegrees * deltaTime / m_xRotationData.m_fTime;
	auto deltaRotator = FRotator(0, yawDt, 0);
	GetPig()->AddActorWorldRotation(deltaRotator);
}

bool URotationController::IsCurrentlyRotating() {
	return m_bIsCurrentlyRotating;
}

float URotationController::GetLinearVelocityMagnitude() {
	return m_pWalkingController->GetWalkingSpeedFor(s_xRotationMovementType);
}

float URotationController::GetAngularVelocity() {
	return m_bIsCurrentlyRotating ?
		m_fAngularVelocity :
		GetPig()->GetPigAnimInstance()->CalculateDirection(GetPig()->GetVelocity(), GetPig()->GetActorRotation());
}

void URotationController::SetWalkingController(AWalkingController* walkingController) {
	m_pWalkingController = walkingController;
}

void URotationController::InitRotationData(const FPigRotationData& rotationData) {
	m_xRotationData.m_dDistance = FVector(rotationData.WalkDeltaX, rotationData.WalkDeltaY, 0).Length();
	m_xRotationData.m_fDegrees = rotationData.DegreesInTime;
	m_xRotationData.m_fTime = rotationData.TimeTaken;

	if(m_bIsCurrentlyRotating) RecalcRotationData();
}

void URotationController::RecalcRotationData() {
	EndPeriod();

	m_fRotationTimeCounter = 0;

	auto pigRotation = GetPig()->GetActorRotation().Yaw + 180;
	auto newRotator = UKismetMathLibrary::FindLookAtRotation(GetPig()->GetActorLocation(), m_xRotatingTo).Yaw + 180;

	auto degreesToRotate = newRotator - pigRotation;

	//degreesToRotate = 90; // for debug

	if(degreesToRotate > 180) {
		degreesToRotate -= 360;
	} else if(degreesToRotate < -180) {
		degreesToRotate += 360;
	}

	if(FMath::Abs(degreesToRotate) <= 2) {
		auto deltaRotator = FRotator(0, degreesToRotate, 0);
		GetPig()->AddActorWorldRotation(deltaRotator);
		EndRotating();
	}

	auto timesToRotate = FMath::Abs(degreesToRotate) / m_xRotationData.m_fDegrees;

	m_fTimeToRotate = timesToRotate * m_xRotationData.m_fTime;

	auto isRotationToRight = degreesToRotate > 0;

	m_iRotationMul = isRotationToRight ? s_iRightRotationMul : s_iLeftRotationMul;
	m_fAngularVelocity = isRotationToRight ? s_iRotationToRight : s_iRotationToLeft;
}

void URotationController::TryEndRotating() {
	RotateTo(m_xRotatingTo);
}

void URotationController::EndRotating() {
	m_bIsCurrentlyRotating = false;
	OnEvent(ERotationControllerEvent::RotationCompleted);
	//RotateTo(FVector::ZeroVector); // for debug
}


void URotationController::EndPeriod() {
	m_fTimePerPeriodCounter = 0.f;
}