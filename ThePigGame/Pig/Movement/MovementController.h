#pragma once

#include "CoreMinimal.h"
#include "MovementType.h"
#include "../../Utils/PigDataUser/IPigDataUser.h"
#include "../../Utils/EventHandler/TEventHandler.h"
#include "MovementControllerEvent.h"
#include "MovementController.generated.h"

UCLASS()
class THEPIGGAME_API UMovementController : public UObject, public ICachedPigDataUser, public TEventHandler<EMovementControllerEvent> {

	GENERATED_BODY()

	struct SRotationPeriodData {
		double m_dDistance = 0.0;
		float m_fTime = 0.f;
		float m_fDegrees = 0.f;
	};

	public:
	float GetVelocity();
	float GetRotation();

	public:
	void InitChildMovementSpeeds();
	void InitAdultMovementSpeeds();

	public:
	void SetMovementType(EMovementType movementType);

	public:
	void Tick(float deltaTime);

	public:
	void RotateTo(const FVector& location);
	void RecalcRotationData();

	protected:
	void InitMovementSpeeds(bool isAdult);
	
	protected:
	void UpdateMaxSpeed();

	protected:
	void EndRotating();

	protected:
	float SpeedToScale(float baseSpeed);
	float SpeedFromScale(float speed);

	

	protected:
	void EndPeriod();

	protected:
	EMovementType m_xCurrentMovementType = EMovementType::Default;

	protected:
	TStaticArray<float, (uint32)EMovementType::Size> m_vMovementSpeeds;

	protected:
	float m_fVelocity = 0.f;
	float m_fRotation = 0.f;

	protected:
	bool m_bIsCurrentlyRotating = false;

	protected:
	float m_fTimeToRotate = 0.f;

	protected:
	float m_fRotationTimeCounter = 0.f;
	float m_fTimePerPeriodCounter = 0.f;

	protected:
	int m_iRotationMul = 1;

	protected:
	SRotationPeriodData m_xRotationData;

	protected:
	FVector m_xRotatingTo = FVector::ZeroVector;

};