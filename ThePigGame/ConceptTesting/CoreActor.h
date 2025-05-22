#pragma once
#include "IRedirectReceiver.h"
//#include "PigGameCore/Test.h"

class RedirectorToUEActor;



class CoreActor : public IRedirectReceiver {
	public:
	CoreActor(RedirectorToUEActor* redirect);
	virtual ~CoreActor() = default;

	//From UE to Core(here)
	virtual void Tick(float delta);

	// From Core(here) to UE
	void SetLocation(FVector location);

	//Test t;

	private:
	RedirectorToUEActor* redirectToUE = nullptr;
};
