#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PigProperties/PigPropertyType.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "ThePigGame/Utils/Property/Property.h"
#include "PropertySubControllerBase.generated.h"

/// Children of this class are the owner of the properties they control
/// No one else can change value of these properties
/// Even using a public function inside of which property would be changed

// todo rename this class into PropertySubControllerBase. Rename folder for this class into PropertySubControllers.
UCLASS(Abstract)
class THEPIGGAME_API UPropertySubControllerBase : public UObject, public INoCachePigDataUser, public IPropertyTickProvider
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

