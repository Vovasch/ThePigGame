#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PigProperties/PigPropertyType.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "ThePigGame/Utils/Property/Property.h"
#include "PropertyControllerBase.generated.h"


UCLASS(Abstract)
class THEPIGGAME_API UPropertyControllerBase : public UObject, public INoCachePigDataUser, public IPropertyTickProvider
{
	GENERATED_BODY()

	public:
	void Init(USupremePropertyController* owner);
	virtual void InitServiceProperties();
	virtual void InitProperties();

	public:
	virtual APig* GetPig() override;

	public:
	virtual void Tick(float delta);

	protected:
	template<EPigPropertyType type>
	auto GetProperty() {
		return m_pOwnerController->GetProperty<type>();
	}

	protected:
	TWeakObjectPtr<USupremePropertyController> m_pOwnerController = nullptr;

};

