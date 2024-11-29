#include "WeightController.h"

#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PigProperties/PigPropertyType.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"

const MaxWeight* UWeightController::GetMaxWeight() {
	return &m_xMaxWeight;
}

const CriticalWeight* UWeightController::GetCriticalWeight() {
	return &m_xCriticalWeight;
}

const Weight* UWeightController::GetWeight() {
	return &m_xWeight;
}

void UWeightController::Tick(float delta) {
	Super::Tick(delta);

	// TODO: WorldCharacteristic won't ever be true because of clamp in property
	if(m_xWeight.GetCurrent() <= m_xCriticalWeight.GetCurrent())
	{
		// confisacate pig
		//OnEvent(EPigEvent::RemovedFromFarm);
	}
}

void UWeightController::CheckBellyful() {

	auto currentBellyful = GetProperty<EPigPropertyType::Bellyful>()->GetCurrent();
	if(currentBellyful >= GetInitData()->BellyfulLevelToGetFat) {
		m_xWeight.SetDelta(m_fWeightDeltaPerTick);
		return;
	}

	if(currentBellyful <= GetInitData()->BellyfulLevelToGetThin) {
		m_xWeight.SetDelta(-m_fWeightDeltaPerTick);
		return;
	}

	m_xWeight.SetDelta(0.f);
}

void UWeightController::InitServiceProperties() {
	m_fWeightDeltaPerTick = (GetInitData()->CriticalWeightAtMaxAge - GetInitData()->CriticalWeightAtMinAge) / GetPropertyController()->GetAgeController()->GetMaxSizesAtSeconds(); // TODO: WorldCharacteristic verify
	m_fWeightDeltaPerTick *= 1.5f;

	m_fWeightDeltaPerTick = 0.5;
}

void UWeightController::InitProperties() {

	auto ageProp = GetProperty<EPigPropertyType::Age>();
	auto maxSizesAtSeconds = m_pOwnerController->GetAgeController()->GetMaxSizesAtSeconds();

	m_xCriticalWeight.Init(this);
	m_xCriticalWeight.GetMinMaxType().SetMinMax(GetInitData()->CriticalWeightAtMinAge, GetInitData()->CriticalWeightAtMaxAge);
	m_xCriticalWeight.CalcCoeff(ageProp->GetCurrentPtr(), 0, maxSizesAtSeconds);

	m_xMaxWeight.Init(this);
	m_xMaxWeight.GetMinMaxType().SetMinMax(GetInitData()->MaxWeightAtMinAge, GetInitData()->MaxWeightAtMaxAge);
	m_xMaxWeight.CalcCoeff(ageProp->GetCurrentPtr(), 0, maxSizesAtSeconds);

	m_xWeight.Init(this);
	m_xWeight.GetMinMaxType().SetMinMax(m_xCriticalWeight.GetCurrentPtr(), m_xMaxWeight.GetCurrentPtr());
	m_xWeight.SetDelta(m_fWeightDeltaPerTick);
	m_xWeight.GetCurrentModifycationType().Set((GetInitData()->MaxWeightAtMinAge - GetInitData()->CriticalWeightAtMinAge) / 2 + GetInitData()->CriticalWeightAtMinAge);
}
