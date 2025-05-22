#pragma once
#include "IRedirector.h"
#include "UEPig.h"


class AUEPig;
class AUEActor;

class RedirectorToUEActor : public IRedirectorToUE {

	public:
	RedirectorToUEActor(AUEActor* actor);
	virtual ~RedirectorToUEActor()=default;
	
	void SetLocation(FVector location);

	private:
	AUEActor* GetRedirectReceiver() override;

};

class RedirectorToUEPig : public RedirectorToUEActor {
	public:

	RedirectorToUEPig(AUEPig* pig);

	/*void SetActor(AUEActor* actor) override {
		m_pPig = Cast<AUEPig>(actor);
	}*/

	void InitSomethingPigRelated();

	private:
	AUEPig* GetRedirectReceiver() override;

};