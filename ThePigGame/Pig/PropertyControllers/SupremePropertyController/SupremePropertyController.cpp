#include "SupremePropertyController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/ConsumingController/ConsumingController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/MiscController/MiscController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/SleepingController/SleepingController.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/WeightController/WeightController.h"

DEFINE_LOG_CATEGORY_STATIC(SupremePropertyControllerLog, Log, All)

USupremePropertyController::USupremePropertyController() {
	// todo add controllers into array. and use CreateDefaultSubObject, because we are in the constructor
	m_pAgeController = NewObject<UAgeController>();
	m_pConsumingController = NewObject<UConsumingController>();
	m_pSleepingController = NewObject<USleepingController>();
	m_pWeightController = NewObject<UWeightController>();
	m_pMiscController = NewObject<UMiscController>();

	m_vProperties[uint32(EPigPropertyType::Age)] = m_pAgeController->GetAge();
	m_vProperties[uint32(EPigPropertyType::IsAdult)] = m_pAgeController->GetIsAdult();
	m_vProperties[uint32(EPigPropertyType::Bellyful)] = m_pConsumingController->GetBellyful();
	m_vProperties[uint32(EPigPropertyType::Thirst)] = m_pConsumingController->GetThirst();
	m_vProperties[uint32(EPigPropertyType::Energy)] = m_pSleepingController->GetEnergy();
	m_vProperties[uint32(EPigPropertyType::MaxWeight)] = m_pWeightController->GetMaxWeight();
	m_vProperties[uint32(EPigPropertyType::CriticalWeight)] = m_pWeightController->GetCriticalWeight();
	m_vProperties[uint32(EPigPropertyType::Weight)] = m_pWeightController->GetWeight();
	m_vProperties[uint32(EPigPropertyType::Scale)] = m_pMiscController->GetScale();
	m_vProperties[uint32(EPigPropertyType::Morph)] = m_pMiscController->GetMorph();


	auto foundNullptr = false;
	for(uint32 i = 0; i < uint32(EPigPropertyType::Size); ++i) {
		if(!m_vProperties[i]) {
			foundNullptr = true;
			UE_LOG(SupremePropertyControllerLog, Error, TEXT("%s property is a nullptr"), *UEnum::GetDisplayValueAsText(EPigPropertyType(i)).ToString());
		}
	}

	if(foundNullptr) {
		UE_LOG(SupremePropertyControllerLog, Fatal, TEXT("One property or more are nullptr"));
	}
}

void USupremePropertyController::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);

	// todo add controllers into array.
	m_pAgeController->Init(this);
	m_pConsumingController->Init(this);
	m_pSleepingController->Init(this);
	m_pWeightController->Init(this);
	m_pMiscController->Init(this);

	m_pAgeController->InitServiceProperties();
	m_pConsumingController->InitServiceProperties();
	m_pSleepingController->InitServiceProperties();
	m_pWeightController->InitServiceProperties();
	m_pMiscController->InitServiceProperties();

	m_pAgeController->InitProperties();
	m_pConsumingController->InitProperties();
	m_pSleepingController->InitProperties();
	m_pWeightController->InitProperties();
	m_pMiscController->InitProperties();
}

UAgeController* USupremePropertyController::GetAgeController() {
	return m_pAgeController;
}

UConsumingController* USupremePropertyController::GetConsumingController() {
	return m_pConsumingController;
}

USleepingController* USupremePropertyController::GetSleepingController() {
	return m_pSleepingController;
}

UWeightController* USupremePropertyController::GetWeightController() {
	return m_pWeightController;
}

UMiscController* USupremePropertyController::GetMiscController() {
	return m_pMiscController;
}

void USupremePropertyController::Tick(float delta) {
	m_pAgeController->Tick(delta);
	m_pConsumingController->Tick(delta);
	m_pSleepingController->Tick(delta);
	m_pWeightController->Tick(delta);
	m_pMiscController->Tick(delta);
}