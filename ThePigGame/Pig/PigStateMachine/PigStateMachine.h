#pragma once

#include "../../Utils/StateMachine/TStateMachine.h"
#include "CoreMinimal.h"
#include "PigStates.h"
#include "../../Utils/PigDataUser/IPigDataUser.h"
#include "PigStateMachine.generated.h"

class APig;

UCLASS()
class THEPIGGAME_API UPigStateMachine : public UObject, public TStateMachine<EPigStates>, public ICachedPigDataUser {
	GENERATED_BODY()

	public:
	UPigStateMachine();

	public:
	virtual void Init(APig* pig) override;

	public:
	void AfterInit();

	public:
	virtual void OnStateChanged(EPigStates oldState, EPigStates newState) override;

	protected:
	void SubscribeOnAnimInstanceController();

	protected:
	void SubscribeOnAnimInstance();
	void UnsubscribeFromAnimInstance();
};
