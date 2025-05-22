#pragma once

#include "UEActor.h"
#include "UEPig.generated.h"

class CorePig;

UCLASS()
class AUEPig : public AUEActor {
	GENERATED_BODY()

	public:
	AUEPig();
	AUEPig(std::unique_ptr<CorePig>&& corePig);

	void InitSomethingPigRelated() {
		
	}

};