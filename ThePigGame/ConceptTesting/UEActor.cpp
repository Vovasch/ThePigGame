#include "UEActor.h"

#include "CoreActor.h"
#include "RedirectorToUEActor.h"


AUEActor::AUEActor() : IRedirectorToCore(std::make_unique<CoreActor>(new RedirectorToUEActor(this))) {
}

AUEActor::AUEActor(std::unique_ptr<CoreActor>&& coreActor) : IRedirectorToCore(std::move(coreActor)) {
}

void AUEActor::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	GetRedirectReceiver()->Tick(DeltaTime);
}

CoreActor* AUEActor::GetRedirectReceiver() {
	return dynamic_cast<CoreActor*>(GetRedirectReceiverBase());
}