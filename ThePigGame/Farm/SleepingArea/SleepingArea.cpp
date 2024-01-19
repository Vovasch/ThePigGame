// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepingArea.h"

// Sets default values for this component's properties
USleepingArea::USleepingArea() {
	
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void USleepingArea::BeginPlay() {
	Super::BeginPlay();


	
}


// Called every frame
void USleepingArea::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USleepingArea::OnPigStartedSleeping(APig* pig) {
	m_vSleepingPigs.Add(pig);
}

void USleepingArea::OnPigEndedSleeping(APig* pig) {
	m_vSleepingPigs.Remove(pig);
}

APig* USleepingArea::GetAnySleepingPig() {
	if(m_vSleepingPigs.IsEmpty()) return nullptr;
	return *m_vSleepingPigs.begin();
}