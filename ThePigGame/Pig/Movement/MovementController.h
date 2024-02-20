#pragma once

#include "CoreMinimal.h"
#include "MovementType.h"
#include "../../Utils/PigDataUser/IPigDataUser.h"
#include "MovementController.generated.h"

UCLASS()
class THEPIGGAME_API UMovementController : public UObject, public ICachedPigDataUser {

	GENERATED_BODY()

	public:
	void InitChildMovementSpeeds();
	void InitAdultMovementSpeeds();

	public:
	void SetMovementType(EMovementType movementType);

	public:
	void UpdateMaxSpeed();

	protected:
	void InitMovementSpeeds(bool isAdult);

	protected:
	EMovementType m_xCurrentMovementType = EMovementType::Default;

	protected:
	TStaticArray<float, (uint32)EMovementType::Size> m_vMovementSpeeds;

};