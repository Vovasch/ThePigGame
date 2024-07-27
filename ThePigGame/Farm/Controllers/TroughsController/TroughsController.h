#pragma once

#include "CoreMinimal.h"
#include "TroughtsControllerEvent.h"
#include "ThePigGame/Farm/Controllers/FarmControllerBase/FarmControllerBase.h"
#include "ThePigGame/Utils/EventHandler/TEventHandler.h"
#include "TroughsController.generated.h"

class UEatingSpot;
class ATrough;

UCLASS()
class THEPIGGAME_API UTroughsController : public UFarmControllerBase, public TEventHandler<ETroughsControllerEvent>{
	GENERATED_BODY()

	public:
	virtual void Init() override;
	
	public:
	UEatingSpot* GetAvailableEatingSpot();

	protected:
	TArray<ATrough*> m_vTroughs;

	protected:
	int32 m_iTotalEatingSpots = 0;
	int32 m_iAvailableEatingSpots = 0;
};