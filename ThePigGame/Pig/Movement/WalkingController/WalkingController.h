#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/PigDataUser/IPigDataUser.h"
#include "WalkingControllerEvent.h"
#include "ThePigGame/Pig/Movement/MovementType.h"
#include "WalkingController.generated.h"

class AAIController;

UCLASS()
class THEPIGGAME_API AWalkingController : public AActor, public ICachedPigDataUser, public TEventHandler<EWalkingControllerEvent> {

	GENERATED_BODY()

	public:
	AWalkingController();

	public:
	virtual void Tick(float DeltaSeconds) override;

	public:
	float GetLinearVelocityMagnitude();

	public:
	void WalkTo(const FVector& loc);
	void InterruptWalking();

	public:
	void InitWalkingData(const FPigWalkingData& initData);

	public:
	float SpeedToScale(float baseSpeed);
	float SpeedFromScale(float speed);

	public:
	void SetWalkingType(EMovementType movementType);

	public:
	float GetWalkingSpeedFor(EMovementType movementType);

	protected:
	void UpdateMaxSpeed();

	protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BPWalkToLocation(const FVector& loc);

	UFUNCTION(BlueprintImplementableEvent)
	void BPInterruptWalking();

	UFUNCTION(BlueprintCallable)
	void OnWalkingCompleted();

	UFUNCTION(BlueprintCallable)
	void OnWalkingFailed();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAIController* GetAIControllerBP();

	protected:
	TStaticArray<float, (uint32)EMovementType::Size> m_vWalkingSpeeds;

	protected:
	EMovementType m_xCurrentWalkingType = EMovementType::Default;
};