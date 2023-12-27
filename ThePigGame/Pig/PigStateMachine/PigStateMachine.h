#pragma once

#include "../../Utils/StateMachine/TStateMachine.h"
#include "CoreMinimal.h"
#include "PigStates.h"
#include "PigStateMachine.generated.h"

class APig;

UCLASS()
class THEPIGGAME_API UPigStateMachine : public UObject, public TStateMachine<EPigStates> {
	GENERATED_BODY()

	public:
	UPigStateMachine();

	public:
	void SetPigOwner(APig* pigOwner);

	protected:
	APig* m_pPigOwner = nullptr;

};
