#pragma once
#include "CoreMinimal.h"


template<typename ParentClass>
class TPigTaskBase {
	public:
	TPigTaskBase()=default;
	TPigTaskBase(FString& parentClassNodeName) {
		parentClassNodeName = ParentClass::StaticClass()->GetFName().ToString();
	}
};