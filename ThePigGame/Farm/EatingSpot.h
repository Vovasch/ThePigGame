#pragma once

#include "CoreMinimal.h"
#include "Engine/ObjectLibrary.h"
#include "../Utils/EventHandler/TEventHandler.h"
#include "EatingSpotEvent.h"
#include "EatingSpot.generated.h"


UCLASS()
class THEPIGGAME_API UEatingSpot : public UObject, public TEventHandler<EEatingSpotEvent> {
	GENERATED_BODY()

	public:
	UEatingSpot()=default;
	UEatingSpot(const FVector& location, bool available=true);

	public:
	bool IsAvailable();
	void SetAvailable(bool isAvailable);

	public:
	const FVector& GetLocation();
	void SetLocation(const FVector& loc);

	protected:
	UPROPERTY(VisibleAnywhere)
	FVector m_xLocation = FVector::ZeroVector;

	protected:
	bool m_bAvailable = true;
};