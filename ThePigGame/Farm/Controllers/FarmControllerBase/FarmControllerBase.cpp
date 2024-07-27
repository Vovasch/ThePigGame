#include "FarmControllerBase.h"
#include "ThePigGame/Farm/Farm.h"

void UFarmControllerBase::Init() {
	
}


AFarm* UFarmControllerBase::GetFarm() {
	return Cast<AFarm>(GetOuter());
}


