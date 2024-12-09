#include "AnimInstanceController.h"

#include "PigAnimInstance.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"


void UAnimInstanceController::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);

	GetPropertyController()->GetSubController<ESubControllerType::Age>()->Subscribe(this, EAgeControllerEvent::BecomeAdult, [this]() {
		ReinitAnimInstance();
	});
}

void UAnimInstanceController::ReinitAnimInstance() {
	OnEvent(EAnimInstanceControllerEvent::OnBeforeDisengaged);

	auto pig = GetPig();

	pig->GetMesh()->SetSkeletalMesh(GetInitData()->m_xAdultMesh, false);
	pig->GetMesh()->SetAnimInstanceClass(GetInitData()->m_xAdultAnimBlueprint);

	pig->GetPigAnimInstance()->Init(pig);

	OnEvent(EAnimInstanceControllerEvent::OnAfterEnabled);
}
