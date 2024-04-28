#pragma once

#include "CoreMinimal.h"
#include "MovementType.h"
#include "../../Utils/PigDataUser/IPigDataUser.h"
#include "../../Utils/EventHandler/TEventHandler.h"
#include "MovementControllerEvent.h"
#include "MovementController.generated.h"

class AWalkingController;
class URotationController;

UCLASS()
class THEPIGGAME_API UMovementController : public UObject, public ICachedPigDataUser, public TEventHandler<EMovementControllerEvent> {

	GENERATED_BODY()

	public:
	UMovementController();
	
	public:
	virtual void Init(APig* pig) override;

	public:
	void MoveTo(const FVector& vec);
	void InterruptMovement();

	public:
	float GetVelocity();
	float GetRotation();

	public:
	void InitChildVelocityData();
	void InitAdultVelocityData();

	public:
	void Tick(float deltaTime);

	protected:
	void InitVelocityData(bool isAdult);

	protected:
	UPROPERTY()
	AWalkingController* m_pWalkingController = nullptr;

	protected:
	UPROPERTY()
	URotationController* m_pRotationController = nullptr;


	protected:
	float m_fLinearVelocityMagnitude = 0.f;
	float m_fAngularVelocity = 0.f;



};