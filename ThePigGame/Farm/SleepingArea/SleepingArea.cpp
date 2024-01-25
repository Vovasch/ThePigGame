// Fill out your copyright notice in the Description page of Project Settings.

#include "SleepingArea.h"

void USleepingArea::OnPigStartedSleeping(APig* pig) {
	m_vSleepingPigs.Add(pig);
}

void USleepingArea::RemovePig(APig* pig) {
	m_vSleepingPigs.Remove(pig);
}

void USleepingArea::OnPigEndedSleeping(APig* pig) {
	RemovePig(pig);
}

void USleepingArea::OnPigRemovedFromFarm(APig* pig) {
	RemovePig(pig);
}

APig* USleepingArea::GetAnySleepingPig() const {
	if(m_vSleepingPigs.IsEmpty()) return nullptr;
	return *m_vSleepingPigs.begin();
}