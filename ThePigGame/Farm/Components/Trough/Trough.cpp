// Fill out your copyright notice in the Description page of Project Settings.


#include "Trough.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"

const static FString s_sEatingSpotName = TEXT("EatingSpot");


// Sets default values
ATrough::ATrough() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TroughMesh = CreateDefaultSubobject<UStaticMeshComponent, UStaticMeshComponent>("TroughMesh");
	SetRootComponent(TroughMesh);

	Info = CreateDefaultSubobject<UTextRenderComponent, UTextRenderComponent>("Info");
	Info->SetupAttachment(GetRootComponent());

}

void ATrough::OnConstruction(const FTransform& Transform) {
	AActor::OnConstruction(Transform);

	
}

// Called when the game starts or when spawned
void ATrough::BeginPlay() {
	Super::BeginPlay();

	m_xFullness.Init();
	m_xFullness.GetMinMaxType().SetMinMax(0, m_pPrototype->m_fCapacity);
	m_xFullness.GetCurrentModifycationType().Set(m_pPrototype->m_fStaringAmount);

	UpdateInfo();

	auto& components = GetComponents();

	for(auto component : components) {
		if(component->GetFName().ToString().Find(s_sEatingSpotName) != -1) {
			m_vEatingSpots.Add(NewObject<UEatingSpot>());
			m_vEatingSpots.Last()->SetLocation((Cast<UStaticMeshComponent>(component)->GetComponentLocation()));
			m_vEatingSpots.Last()->SetOwnerTrough(this);

		}
	}
	
}

void ATrough::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateInfo();
}

void ATrough::Fill(float amount) {
	m_xFullness.GetCurrentModifycationType().Add(amount);

	UpdateInfo();
}

float ATrough::TryEatOut(float amount) {
	auto successEatOut = 0.f;
	auto current = m_xFullness.GetCurrent();

	if(amount >= current) {
		successEatOut = amount;
	} else {
		successEatOut = current;
	}

	m_xFullness.GetCurrentModifycationType().Add(-successEatOut);

	return successEatOut;
}


void ATrough::UpdateInfo() {
	FString str;

	str += FString::SanitizeFloat(m_xFullness.GetCurrent());
	str += " / ";
	str += FString::SanitizeFloat(m_xFullness.GetMinMaxType().GetMax());

	Info->SetText(FText::FromString(str));

}

UEatingSpot* ATrough::GetAvailableEatingSpot() {
	for(auto& spot : m_vEatingSpots) {
		if(spot->IsAvailable()) {
			return spot;
		}
	}
	return nullptr;
}

TArray<UEatingSpot*>& ATrough::GetAllEatingSpots() {
	return m_vEatingSpots;
}