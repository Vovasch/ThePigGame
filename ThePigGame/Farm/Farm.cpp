// Fill out your copyright notice in the Description page of Project Settings.


#include "Farm.h"
#include "../Pig/Pig.h"
#include "../Pig/PigStateMachine/PigStateMachine.h"
#include "EatingSpot.h"
#include "Engine/World.h"
#include "Trough.h"
#include "SleepingArea/SleepingArea.h"

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

	// get pointer to Sleeping Area

	m_pSleepingArea = GetComponentByClass<USleepingArea>();


	// spawn pigs
	{
		FVector origin;
		FVector boxExtent;
		this->GetActorBounds(false, origin, boxExtent);

		auto uclass = LoadObject<UClass>(nullptr, UTF8_TO_TCHAR("Blueprint'/Game/Pig/BP_Pig.BP_Pig_C'"));

		origin.Z = 87.2f;
		for(int i = 0; i < 5; i++) {
			auto pig = GetWorld()->SpawnActor<APig>(uclass, origin, FRotator::ZeroRotator);
			if(!pig) continue;
			origin.X += 70;
			m_vPigs.Add(pig);
			pig->SetOwner(this);
			BindOnPig(pig);
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


const USleepingArea* AFarm::GetSleepingArea() {
	return m_pSleepingArea;
}

void AFarm::BindOnPig(APig* pig) {
	pig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Subscribe(EStateEvent::Start, [this, pig]() {
		m_pSleepingArea->OnPigStartedSleeping(pig);
	});

	pig->GetPigStateMachine()->GetState(EPigStates::Sleeping)->Subscribe(EStateEvent::End, [this, pig]() {
		m_pSleepingArea->OnPigEndedSleeping(pig);
	});
}
