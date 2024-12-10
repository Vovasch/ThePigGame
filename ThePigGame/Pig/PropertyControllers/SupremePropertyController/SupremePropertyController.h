#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PigProperties/PigProperties.h"
#include "ThePigGame/Pig/PigProperties/PigPropertyType.h"
#include "ThePigGame/Pig/PropertyControllers/SubControllerType.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/ConsumingController/ConsumingController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/MiscController/MiscController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/SleepingController/SleepingController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/WeightController/WeightController.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "SupremePropertyController.generated.h"

class UPropertySubControllerBase;
class UConsumingController;
class IProperty;
class UMiscController;
class UAgeController;
class UWeightController;
class USleepingController;

/// When adding new property don't forget to add it in
///	1) GetProperty 2) Constructor
/// Well, in both cases are supposed to be warned by fatal or compile error, but anyways

UCLASS()
class THEPIGGAME_API USupremePropertyController : public UObject, public ICachedPigDataUser
{
	GENERATED_BODY()

	public:
	USupremePropertyController();

	public:
	virtual void Init(APig* pig) override;
	void Tick(float delta);

	public:
	// If you have errors using this function, you might need to add property return for some Type
	template<EPigPropertyType Type>
	const auto* GetProperty() {
		
		if constexpr(Type == EPigPropertyType::Age) return dynamic_cast<const Age*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::IsAdult) return dynamic_cast<const TBoolProperty*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::Bellyful) return dynamic_cast<const ConsumeProperty*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::Hydrated) return dynamic_cast<const ConsumeProperty*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::Energy) return dynamic_cast<const Energy*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::MaxWeight) return dynamic_cast<const MaxWeight*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::CriticalWeight) return dynamic_cast<const CriticalWeight*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::Weight) return dynamic_cast<const Weight*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::Scale) return dynamic_cast<const Scale*>(m_vProperties[uint32(Type)]);
		else if constexpr(Type == EPigPropertyType::Morph) return dynamic_cast<const Morph*>(m_vProperties[uint32(Type)]);
		else {} // no return => compile error. Add return for the property Type
	}

	template<ESubControllerType Type>
	auto* GetSubController() {
		
		if constexpr(Type == ESubControllerType::Age) return Cast<UAgeController>(m_vSubControllers[uint32(Type)].Get());
		else if constexpr(Type == ESubControllerType::Consuming) return Cast<UConsumingController>(m_vSubControllers[uint32(Type)].Get());
		else if constexpr(Type == ESubControllerType::Sleeping) return Cast<USleepingController>(m_vSubControllers[uint32(Type)].Get());
		else if constexpr(Type == ESubControllerType::Weight) return Cast<UWeightController>(m_vSubControllers[uint32(Type)].Get());
		else if constexpr(Type == ESubControllerType::Misc) return Cast<UMiscController>(m_vSubControllers[uint32(Type)].Get());
		else {} // no return => compile error. Add return for the sub controller Type
	}

	private:
	TStaticArray<TStrongObjectPtr<UPropertySubControllerBase>, uint32(ESubControllerType::Size)> m_vSubControllers {InPlace, nullptr};
	TStaticArray<const IProperty*, uint32(EPigPropertyType::Size)> m_vProperties {InPlace, nullptr};
};