#pragma once

#include "CoreMinimal.h"
#include "CoreActor.h"
#include "IRedirector.h"
#include "IRedirectReceiver.h"
#include "RedirectorToCore.h"
#include "UEActor.generated.h"


UCLASS()
class AUEActor : public AActor, public IRedirectReceiver, public IRedirectorToCore {
	GENERATED_BODY()

	public:
	AUEActor();
	AUEActor(std::unique_ptr<CoreActor>&& coreActor);

	void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	CoreActor* GetRedirectReceiver() override;
};