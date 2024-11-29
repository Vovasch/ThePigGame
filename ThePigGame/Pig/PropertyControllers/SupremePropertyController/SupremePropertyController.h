#pragma once

#include "CoreMinimal.h"
#include "ThePigGame/Pig/PigProperties/PigProperties.h"
#include "ThePigGame/Pig/PigProperties/PigPropertyType.h"
#include "ThePigGame/Utils/PigDataUser/IPigDataUser.h"
#include "SupremePropertyController.generated.h"

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

	public:
	UAgeController* GetAgeController();
	UConsumingController* GetConsumingController();
	USleepingController* GetSleepingController();
	UWeightController* GetWeightController();
	UMiscController* GetMiscController();

	void Tick(float delta);

	public:
	// If you have errors using this function, you might need to add property return for some type
	template<EPigPropertyType type>
	const auto* GetProperty() {
		
		if constexpr(type == EPigPropertyType::Age) return dynamic_cast<const Age*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::IsAdult) return dynamic_cast<const TBoolProperty*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::Bellyful) return dynamic_cast<const ConsumeProperty*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::Hydrated) return dynamic_cast<const ConsumeProperty*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::Energy) return dynamic_cast<const Energy*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::MaxWeight) return dynamic_cast<const MaxWeight*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::CriticalWeight) return dynamic_cast<const CriticalWeight*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::Weight) return dynamic_cast<const Weight*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::Scale) return dynamic_cast<const Scale*>(m_vProperties[uint32(type)]);
		else if constexpr(type == EPigPropertyType::Morph) return dynamic_cast<const Morph*>(m_vProperties[uint32(type)]);
		else {}// no return => compile error. Add return for the property type
	}

	protected:
	UPROPERTY()
	UAgeController* m_pAgeController = nullptr;

	UPROPERTY()
	UConsumingController* m_pConsumingController = nullptr;

	UPROPERTY()
	USleepingController* m_pSleepingController = nullptr;

	UPROPERTY()
	UWeightController* m_pWeightController = nullptr;

	UPROPERTY()
	UMiscController* m_pMiscController = nullptr;

	protected:
	TStaticArray<const IProperty*, uint32(EPigPropertyType::Size)> m_vProperties {InPlace, nullptr};
};