#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Farm/Controllers/FarmControllerBase/FarmControllerBase.h"
#include "SleepingPigsController.generated.h"

class APig;

UCLASS()
class THEPIGGAME_API USleepingPigsController : public UFarmControllerBase {
	GENERATED_BODY()

	public:
	void RegisterPig(APig* pig);

	protected:
	void OnPigStartedSleeping(APig* pig);
	void OnPigEndedSleeping(APig* pig);
	void OnPigRemovedFromFarm(APig* pig);

	protected:
	void RemovePig(APig* pig);

	public:
	APig* GetAnySleepingPig() const;

	protected:
	TSet<APig*> m_vSleepingPigs;
};