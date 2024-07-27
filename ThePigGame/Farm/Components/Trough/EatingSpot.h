#pragma once

#include "CoreMinimal.h"
#include "EatingSpotEvent.h"
#include "ThePigGame/Utils/EventHandler/TEventHandler.h"
#include "EatingSpot.generated.h"


class ATrough;

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
	void SetOwnerTrough(ATrough* trough);
	ATrough* GetOwnerTrough();

	public:
	const FVector& GetLocation();
	void SetLocation(const FVector& loc);

	protected:
	UPROPERTY(VisibleAnywhere)
	FVector m_xLocation = FVector::ZeroVector;

	TWeakObjectPtr<ATrough> m_pTroughOwner = nullptr;

	protected:
	bool m_bAvailable = true;
};