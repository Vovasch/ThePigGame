#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "VisualInfoController.generated.h"

UCLASS()
class UVisualInfoController : public UObject, public ICachedPigDataUser {
	GENERATED_BODY()

	public:
	void Tick(float deltaTime);

	private:
	void FillInfo();
};