#include "SupremePropertyController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/ConsumingController/ConsumingController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/MiscController/MiscController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/SleepingController/SleepingController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/WeightController/WeightController.h"
#include <functional>

DEFINE_LOG_CATEGORY(SupremePropertyControllerLog)

USupremePropertyController::USupremePropertyController() {
	m_vSubControllers[uint32(ESubControllerType::Age)] = TStrongObjectPtr(CreateDefaultSubobject<UAgeController>("AgeSubController"));
	m_vSubControllers[uint32(ESubControllerType::Consuming)] = TStrongObjectPtr(CreateDefaultSubobject<UConsumingController>("ConsumingSubController"));
	m_vSubControllers[uint32(ESubControllerType::Sleeping)] = TStrongObjectPtr(CreateDefaultSubobject<USleepingController>("SleepingSubController"));
	m_vSubControllers[uint32(ESubControllerType::Weight)] = TStrongObjectPtr(CreateDefaultSubobject<UWeightController>("WeightSubController"));
	m_vSubControllers[uint32(ESubControllerType::Misc)] = TStrongObjectPtr(CreateDefaultSubobject<UMiscController>("MiscSubController"));

	m_vProperties[uint32(EPigPropertyType::Age)] = GetSubController<ESubControllerType::Age>()->GetAge();
	m_vProperties[uint32(EPigPropertyType::IsAdult)] = GetSubController<ESubControllerType::Age>()->GetIsAdult();
	m_vProperties[uint32(EPigPropertyType::Bellyful)] = GetSubController<ESubControllerType::Consuming>()->GetBellyful();
	m_vProperties[uint32(EPigPropertyType::Hydrated)] = GetSubController<ESubControllerType::Consuming>()->GetHydrated();
	m_vProperties[uint32(EPigPropertyType::Energy)] = GetSubController<ESubControllerType::Sleeping>()->GetEnergy();
	m_vProperties[uint32(EPigPropertyType::MaxWeight)] = GetSubController<ESubControllerType::Weight>()->GetMaxWeight();
	m_vProperties[uint32(EPigPropertyType::CriticalWeight)] = GetSubController<ESubControllerType::Weight>()->GetCriticalWeight();
	m_vProperties[uint32(EPigPropertyType::Weight)] = GetSubController<ESubControllerType::Weight>()->GetWeight();
	m_vProperties[uint32(EPigPropertyType::Scale)] = GetSubController<ESubControllerType::Misc>()->GetScale();
	m_vProperties[uint32(EPigPropertyType::Morph)] = GetSubController<ESubControllerType::Misc>()->GetMorph();

	VerifyContainerElements<ESubControllerType>(m_vSubControllers);
	VerifyContainerElements<EPigPropertyType>(m_vProperties);
}

void USupremePropertyController::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);

	auto functions = {
		std::function([this](TStrongObjectPtr<UPropertySubControllerBase> controller) {
				controller->Init(this);
		}),
		std::function([](TStrongObjectPtr<UPropertySubControllerBase> controller) {
				controller->InitServiceProperties();
		}),
		std::function([](TStrongObjectPtr<UPropertySubControllerBase> controller) {
				controller->InitProperties();
		})
	};

	for(auto& func : functions) {
		std::for_each(m_vSubControllers.begin(), m_vSubControllers.end(), func);
	}
}

void USupremePropertyController::Tick(float delta) {
	for(auto controller : m_vSubControllers) {
		controller->Tick(delta);
	}
}