#include "UEPig.h"
#include "CorePig.h"
#include "RedirectorToUEActor.h"


AUEPig::AUEPig() : AUEActor(std::make_unique<CorePig>(new RedirectorToUEPig(this))) {
	
}

AUEPig::AUEPig(std::unique_ptr<CorePig>&& corePig) : AUEActor(std::move(corePig)) {
	
}

