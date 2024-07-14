#include "AgeController.h"

#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "ThePigGame/Levels/MainLevel/MainScreenGameMode.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/AnimInstance/PigAnimInstance.h"

const Age* UAgeController::GetAge() {
	return &m_xAge;
}

const TBoolProperty* UAgeController::GetIsAdult() {
	return &m_bIsAdult;
}

void UAgeController::InitServiceProperties() {
	auto gameMode = Cast<AMainScreenGameMode>(GetPig()->GetWorld()->GetAuthGameMode());

	m_fMaxAgeAtSeconds = GetInitData()->MaxAgeYears * gameMode->GetOneYearInSeconds();
	m_fMaxSizesAtSeconds = GetInitData()->MaxSizesAtAgeYears * gameMode->GetOneYearInSeconds();
	m_fAgeWhenAdultSeconds = GetInitData()->AgeOfAdultYears * gameMode->GetOneYearInSeconds();	
}


void UAgeController::InitProperties() {
	m_xAge.Init(this);
	m_xAge.GetMinMaxType().SetMinMax(0, m_fMaxAgeAtSeconds);
}

float UAgeController::GetMaxSizesAtSeconds() {
	return m_fMaxSizesAtSeconds;
}

float UAgeController::GetAgeWhenAdultSeconds() {
	return m_fAgeWhenAdultSeconds;
}

void UAgeController::Tick(float delta) {
	Super::Tick(delta);
	CheckIfAdult();

	if(m_xAge.GetCurrent() >= m_fMaxAgeAtSeconds)
	{
		// todo won't fire because of clamp in property
		OnEvent(EAgeControllerEvent::BecomeOld);
		// take pig out of bussiness into hospice
		//OnEvent(EPigEvent::RemovedFromFarm);
	}
}


void UAgeController::CheckIfAdult() {

	if(!m_bIsAdult.GetValue() && m_xAge.GetCurrent() >= m_fAgeWhenAdultSeconds) {
		m_bIsAdult = true;

		OnEvent(EAgeControllerEvent::BecomeAdult);
	}
}