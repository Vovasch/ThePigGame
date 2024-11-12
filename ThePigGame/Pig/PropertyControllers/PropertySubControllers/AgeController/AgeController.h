#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PigProperties/PigProperties.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/PropertySubControllerBase.h"
#include "AgeControllerEvent.h"
#include "ThePigGame/Utils/EventHandler/TEventHandler.h"
#include "AgeController.generated.h"

UCLASS()
class THEPIGGAME_API UAgeController : public UPropertySubControllerBase, public TEventHandler<EAgeControllerEvent> {
	GENERATED_BODY()

	public:
	const Age* GetAge();
	const TBoolProperty* GetIsAdult();

	public:
	float GetMaxSizesAtSeconds();
	float GetAgeWhenAdultSeconds();

	public:
	virtual void Tick(float delta) override;

	public:
	virtual void InitServiceProperties() override;
	virtual void InitProperties() override;

	protected:
	void CheckIfAdult();

	protected:
	Age m_xAge;
	TBoolProperty m_bIsAdult = false;

	protected:
	float m_fMaxAgeAtSeconds = 0.f;
	float m_fMaxSizesAtSeconds = 0.f;
	float m_fAgeWhenAdultSeconds = 0.f;
};