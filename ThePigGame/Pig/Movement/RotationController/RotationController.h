#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "RotationControllerEvent.h"
#include "RotationController.generated.h"

class AWalkingController;

UCLASS()
class THEPIGGAME_API URotationController : public UObject, public ICachedPigDataUser, public TEventHandler<ERotationControllerEvent> {

	GENERATED_BODY()

	struct SRotationPeriodData {
		double m_dDistance = 0.0;
		float m_fTime = 0.f;
		float m_fDegrees = 0.f;
	};

	public:
	void RotateTo(const FVector& vec);

	public:
	void Tick(float delta);

	public:
	bool IsCurrentlyRotating();

	public:
	float GetLinearVelocityMagnitude();
	float GetAngularVelocity();

	public:
	void SetWalkingController(AWalkingController* walkingController);

	public:
	void InitRotationData(const FPigRotationData& rotationData);

	protected:
	void RecalcRotationData();

	protected:
	void TryEndRotating();
	void EndRotating();

	protected:
	void EndPeriod();

	protected:
	float m_fAngularVelocity = 0.f;

	protected:
	AWalkingController* m_pWalkingController = nullptr;

	protected:
	FVector m_xRotatingTo = FVector::ZeroVector;
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
};