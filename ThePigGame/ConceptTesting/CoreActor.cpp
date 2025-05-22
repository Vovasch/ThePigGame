#include "CoreActor.h"

#include "RedirectorToUEActor.h"

CoreActor::CoreActor(RedirectorToUEActor* redirect) : redirectToUE(redirect)
{
}

void CoreActor::Tick(float delta) {
}

void CoreActor::SetLocation(FVector location) {
	redirectToUE->SetLocation(location);
}
