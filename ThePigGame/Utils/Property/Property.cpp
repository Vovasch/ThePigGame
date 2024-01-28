#include "Property.h"



float TNoMinMax::ClampNewCurrent(float newCurrent) {
	return newCurrent;
}

float THasMinMax::ClampNewCurrent(float newCurrent) {
	return FMath::Clamp(newCurrent, GetMin(), GetMax());
}

float TMinMaxStatic::GetMax() {
	return m_fMax;
}

float TMinMaxStatic::GetMin() {
	return m_fMin;
}

void TMinMaxStatic::SetMinMax(float min, float max) {
	m_fMin = min;
	m_fMax = max;
}

void TMinMaxDynamic::SetMinMax(const float* min, const float* max) {
	m_fMin = min;
	m_fMax = max;
}

float TMinMaxDynamic::GetMin() {
	return *m_fMin;
}

float TMinMaxDynamic::GetMax() {
	return *m_fMax;
}


void TNoTickType::Init(IPropertyTickProvider* tickProvider) {
	// don't subscribe, TNoTickType means no need to tick.
}

void TNoTickType::Tick(float delta) {
	// TNoTickType, so no need to tick.
}

void TNoTickType::Subscribe(TTickingFunction function) {
	// do nothing
}

void TInstantTicking::Tick(float delta) {
	for(auto& func : m_vSubs)
		func(delta);
}

void TInstantTicking::Init(IPropertyTickProvider* tickProvider) {
	m_pTickProvider = tickProvider;
	m_pTickProvider->SubscribeOnTick(this);
}

void TInstantTicking::Subscribe(TTickingFunction function) {
	m_vSubs.Add(function);
}

void TEventualTicking::Init(IPropertyTickProvider* tickProvider) {
	m_pTickProvider = tickProvider;
}

void TEventualTicking::StartTicking() {
	m_pTickProvider->SubscribeOnTick(this);
}

void TEventualTicking::StopTicking() {
	m_pTickProvider->UnsubscribeFromTick(this);
}

void TConditionalTicking::Tick(float delta) {
	if(m_xCondition()) TInstantTicking::Tick(delta);
}

void TConditionalTicking::SetCondition(TCondition condition) {
	m_xCondition = condition;
}

void IPropertyTickProvider::SubscribeOnTick(TInstantTicking* tickSub) {
	m_xSubs.Add(tickSub);
}

void IPropertyTickProvider::UnsubscribeFromTick(TInstantTicking* tickSub) {
	for(auto sub : m_xSubs) {
		if(sub == tickSub) {
			m_xSubs.Remove(sub);
			break;
		}
	}
}

void IPropertyTickProvider::TickProviderTick(float delta) {
	for(auto sub : m_xSubs)
		sub->Tick(delta);
}


void TAllowedCurrentModification::Set(float value) const {
	m_xSetter(value);
}

void TAllowedCurrentModification::Add(float value) const {
	m_xAdder(value);
}

void TAllowedCurrentModification::SetSetter(TSetter setter) {
	m_xSetter = setter;
}

void TAllowedCurrentModification::SetAdder(TAdder adder) {
	m_xAdder = adder;
}
