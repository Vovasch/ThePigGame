#pragma once

#include "CoreMinimal.h"
#include "AnimInstanceControllerEvent.h"
#include "ThePigGame/Utils/EventHandler/TEventHandler.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "AnimInstanceController.generated.h"

class APig;

UCLASS()
class THEPIGGAME_API UAnimInstanceController : public UObject, public TEventHandler<EAnimInstanceControllerEvent>, public ICachedPigDataUser {
	GENERATED_BODY()

	public:
	virtual void Init(APig* pig) override;

	protected:
	void ReinitAnimInstance();
};