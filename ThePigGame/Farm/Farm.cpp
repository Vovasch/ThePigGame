// Fill out your copyright notice in the Description page of Project Settings.


#include "Farm.h"
#include "../Pig/Pig.h"
#include "EatingSpot.h"
#include "Engine/World.h"
#include "Trough.h"

// Sets default values
AFarm::AFarm() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFarm::BeginPlay() {
	Super::BeginPlay();
	
	// get all troughs to be able to control them.
	{

		TArray<AActor*> actors;
		GetAllChildActors(actors);

		m_iAvailableEatingSpots = m_iTotalEatingSpots = 0;

		for(auto actor : actors) {
			if(auto trough = Cast<ATrough>(actor)) {
				m_vTroughs.Add(trough);

				auto& eatingSpots = trough->GetAllEatingSpots();

				m_iTotalEatingSpots += eatingSpots.Num();

				for(auto eatingSpot : eatingSpots) {
					eatingSpot->Subscribe(EEatingSpotEvent::Occupied, [this]() {
						--m_iAvailableEatingSpots;
					});

					eatingSpot->Subscribe(EEatingSpotEvent::Freed, [this]() {
						auto noneWereAvailable = m_iAvailableEatingSpots == 0;

						++m_iAvailableEatingSpots;
						if(noneWereAvailable) OnEvent(EFarmEvent::EatingSpotFreed);

					});
				}
			}
		}

		m_iAvailableEatingSpots = m_iTotalEatingSpots;

	}

	// spawn pigs
	{
		FVector origin;
		FVector boxExtent;
		this->GetActorBounds(false, origin, boxExtent);

		auto uclass = LoadObject<UClass>(nullptr, UTF8_TO_TCHAR("Blueprint'/Game/Pig/BP_BigPig.BP_BigPig_C'"));

		origin.Z = 87.2f;
		for(int i = 0; i < 4; i++) {
			auto pig = GetWorld()->SpawnActor<APig>(uclass, origin, FRotator::ZeroRotator);
			origin.X += 70;
			if(!pig) continue;
			m_vPigs.Add(pig);
			pig->SetOwner(this);
		}
	}



}

// Called every frame
void AFarm::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

UEatingSpot* AFarm::GetAvailableEatingSpot() {
	for(auto& trough : m_vTroughs) {
		auto eatingSpot = trough->GetAvailableEatingSpot();
		if(eatingSpot) return eatingSpot;
	}

	return nullptr;
}