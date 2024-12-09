#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "ThePigGame/Utils/Property/Property.h"
#include "PropertySubControllerBase.generated.h"

/// Children of this class are the owner of the properties they control
/// No one else can change value of these properties
/// Even using a public function inside of which property would be changed

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

	public:
	USupremePropertyController* GetSupremePropertyController();

	private:
	TWeakObjectPtr<USupremePropertyController> m_pOwnerController = nullptr;

};

