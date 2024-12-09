#include "MiscController.h"

#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"

const Scale* UMiscController::GetScale() {
	return &m_xScale;
}

const Morph* UMiscController::GetMorph() {
	return &m_xMorph;
}

void UMiscController::InitProperties() {

	m_xScale.Init(this);
	m_xScale.GetMinMaxType().SetMinMax(GetInitData()->ChildMinScale, GetInitData()->ChildMaxScale);

	auto ageProp = GetSupremePropertyController()->GetProperty<EPigPropertyType::Age>();
	m_xScale.CalcCoeff(ageProp->GetCurrentPtr(), 0, GetSupremePropertyController()->GetSubController<ESubControllerType::Age>()->GetAgeWhenAdultSeconds());

	m_xMorph.Init(this);
	m_xMorph.GetMinMaxType().SetMinMax(GetInitData()->MorphTargetMinValue, GetInitData()->MorphTargetMaxValue);
	m_xMorph.CalcCoeff(GetSupremePropertyController()->GetProperty<EPigPropertyType::Weight>()->GetCurrentPtr(),
					   GetSupremePropertyController()->GetProperty<EPigPropertyType::CriticalWeight>()->GetCurrentPtr(),
					   GetSupremePropertyController()->GetProperty<EPigPropertyType::MaxWeight>()->GetCurrentPtr());

	GetSupremePropertyController()->GetSubController<ESubControllerType::Age>()->Subscribe(this, EAgeControllerEvent::BecomeAdult, [this]() {
		m_xScale.GetMinMaxType().SetMinMax(GetInitData()->AdultMinScale, GetInitData()->AdultMaxScale);

		auto ageProperty = GetSupremePropertyController()->GetProperty<EPigPropertyType::Age>();

		auto ageController = GetSupremePropertyController()->GetSubController<ESubControllerType::Age>();
		m_xScale.CalcCoeff(ageProperty->GetCurrentPtr(), ageController->GetAgeWhenAdultSeconds(), ageController->GetMaxSizesAtSeconds());
	});
}

void UMiscController::Tick(float delta) {
	Super::Tick(delta);
	SetPigScale();
	SetMeshMorphs();
}

void UMiscController::SetPigScale() {
	auto currScale = m_xScale.GetCurrent();
	//GetMesh()->SetWorldScale3D({ 1,1,1 });
	GetPig()->GetMesh()->SetWorldScale3D({ currScale, currScale, currScale });
}

void UMiscController::SetMeshMorphs() {
	GetPig()->GetMesh()->SetMorphTarget(TEXT("Pig_Big"), m_xMorph.GetCurrent());
}