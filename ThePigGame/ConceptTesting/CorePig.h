#pragma once


#include "CoreActor.h"


class RedirectorToUEPig;

class CorePig : public CoreActor {

	public:
	CorePig(RedirectorToUEPig* redirect);

	void SomeLogic();

	private:
	RedirectorToUEPig* m_pRedirectorToUEPig = nullptr;
};
