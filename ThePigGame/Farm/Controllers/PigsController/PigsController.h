#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Farm/Controllers/FarmControllerBase/FarmControllerBase.h"
#include "PigsController.generated.h"

class APig;

UCLASS()
class THEPIGGAME_API UPigsController : public UFarmControllerBase {
	GENERATED_BODY()

	public:
	void BeginPlay();

	protected:
	void SpawnTestPigs();

	protected:
	void AddPigToFarm(APig* pig);

	protected:
	UPROPERTY()
	TArray<APig*> m_vPigs;
};
