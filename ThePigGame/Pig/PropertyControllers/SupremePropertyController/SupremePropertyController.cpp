#include "SupremePropertyController.h"

#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/EatingController/EatingController.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/MiscController/MiscController.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/SleepingController/SleepingController.h"
#include "ThePigGame/Pig/PropertyControllers/SubPropertyControllers/WeightController/WeightController.h"

USupremePropertyController::USupremePropertyController() {
	m_pAgeController = NewObject<UAgeController>();
	m_pEatingController = NewObject<UEatingController>();
	m_pSleepingController = NewObject<USleepingController>();
	m_pWeightController = NewObject<UWeightController>();
	m_pMiscController = NewObject<UMiscController>();

	m_vProperties.SetNum(uint32(EPigPropertyType::Size));

	m_vProperties[uint32(EPigPropertyType::Age)] = m_pAgeController->GetAge();
	m_vProperties[uint32(EPigPropertyType::IsAdult)] = m_pAgeController->GetIsAdult();
	m_vProperties[uint32(EPigPropertyType::Bellyful)] = m_pEatingController->GetBellyfull();
	m_vProperties[uint32(EPigPropertyType::Energy)] = m_pSleepingController->GetEnergy();
	m_vProperties[uint32(EPigPropertyType::MaxWeight)] = m_pWeightController->GetMaxWeight();
	m_vProperties[uint32(EPigPropertyType::CriticalWeight)] = m_pWeightController->GetCriticalWeight();
	m_vProperties[uint32(EPigPropertyType::Weight)] = m_pWeightController->GetWeight();
	m_vProperties[uint32(EPigPropertyType::Scale)] = m_pMiscController->GetScale();
	m_vProperties[uint32(EPigPropertyType::Morph)] = m_pMiscController->GetMorph();
}

void USupremePropertyController::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);

	m_pAgeController->Init(this);
	m_pEatingController->Init(this);
	m_pSleepingController->Init(this);
	m_pWeightController->Init(this);
	m_pMiscController->Init(this);

	m_pAgeController->InitServiceProperties();
	m_pEatingController->InitServiceProperties();
	m_pSleepingController->InitServiceProperties();
	m_pWeightController->InitServiceProperties();
	m_pMiscController->InitServiceProperties();

	m_pAgeController->InitProperties();
	m_pEatingController->InitProperties();
	m_pSleepingController->InitProperties();
	m_pWeightController->InitProperties();
	m_pMiscController->InitProperties();
}

UAgeController* USupremePropertyController::GetAgeController() {
	return m_pAgeController;
}

UEatingController* USupremePropertyController::GetEatingController() {
	return m_pEatingController;
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
	m_pEatingController->Tick(delta);
	m_pSleepingController->Tick(delta);
	m_pWeightController->Tick(delta);
	m_pMiscController->Tick(delta);
}