#include "RedirectorToUEActor.h"

#include "UEActor.h"

RedirectorToUEActor::RedirectorToUEActor(AUEActor* actor) : IRedirectorToUE(actor)
{

}

void RedirectorToUEActor::SetLocation(FVector location)
{
	GetRedirectReceiver()->SetActorLocation(location);
}

AUEActor* RedirectorToUEActor::GetRedirectReceiver() {
	return dynamic_cast<AUEActor*>(GetRedirectReceiverBase());
}

RedirectorToUEPig::RedirectorToUEPig(AUEPig* pig) : RedirectorToUEActor(pig) {

}

void RedirectorToUEPig::InitSomethingPigRelated() {
	GetRedirectReceiver()->InitSomethingPigRelated();
}

AUEPig* RedirectorToUEPig::GetRedirectReceiver() {
	return dynamic_cast<AUEPig*>(GetRedirectReceiverBase());
}
