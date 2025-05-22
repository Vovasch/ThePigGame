#include "CorePig.h"

#include "RedirectorToUEActor.h"

CorePig::CorePig(RedirectorToUEPig* redirect)  : CoreActor(redirect), m_pRedirectorToUEPig(redirect)
{

}


void CorePig::SomeLogic() {
	m_pRedirectorToUEPig->InitSomethingPigRelated();
}
