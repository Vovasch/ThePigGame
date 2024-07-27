#pragma once

#include "CoreMinimal.h"
#include "FarmControllerBase.generated.h"

class AFarm;

UCLASS()
class THEPIGGAME_API UFarmControllerBase : public UObject {
	GENERATED_BODY()

	public:
	virtual void Init();
	AFarm* GetFarm();

};