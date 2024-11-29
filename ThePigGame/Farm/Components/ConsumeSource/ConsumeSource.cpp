#include "ConsumeSource.h"
#include "ConsumeSourcePrototype.h"
#include "Components/TextRenderComponent.h"
#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Farm/Components/ConsumeSpotComponent/ConsumeSpotComponent.h"
#include "ThePigGame/Farm/Controllers/ConsumeSpotsController/ConsumeSpotsController.h"

DEFINE_LOG_CATEGORY_STATIC(ConsumeSourceLog, Log, All)

AConsumeSource::AConsumeSource() {

	Mesh = CreateDefaultSubobject<UStaticMeshComponent, UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	Info = CreateDefaultSubobject<UTextRenderComponent, UTextRenderComponent>("Info");
	Info->SetupAttachment(RootComponent);
}

void AConsumeSource::BeginPlay() {
	Super::BeginPlay();

	InitOwner();

	InitConsumeSpots();

	InitProperties();

	UpdateInfo();
}

void AConsumeSource::InitOwner() {
	auto owner = GetParentActor();
	if(!owner) {
		UE_LOG(ConsumeSourceLog, Fatal, TEXT("AConsume source supposed to be created from a child actor component"))
	}
	SetOwner(owner);
}

void AConsumeSource::InitConsumeSpots() {
	auto& components = GetComponents();
	auto consumeSpotsController = GetOwnerFarm()->GetConsumeSpotsController();
	
	for(auto component : components) {

		auto asSpotComponent = Cast<UConsumeSpotComponent>(component);
		if(!asSpotComponent) continue;

		consumeSpotsController->AddConsumeSpot(asSpotComponent);
		m_vConsumeSpots.Add(asSpotComponent);
	}
}

void AConsumeSource::InitProperties() {
	m_xFullness.Init();
	m_xFullness.GetMinMaxType().SetMinMax(0, m_pPrototype->m_fCapacity);
	m_xFullness.GetCurrentModifycationType().Set(m_pPrototype->m_fStaringAmount);
}


AFarm* AConsumeSource::GetOwnerFarm() {
	return Cast<AFarm>(GetOwner());
}

void AConsumeSource::UpdateInfo() {
	FString str;

	str += FString::SanitizeFloat(m_xFullness.GetCurrent());
	str += " / ";
	str += FString::SanitizeFloat(m_xFullness.GetMinMaxType().GetMax());

	Info->SetText(FText::FromString(str));

}

void AConsumeSource::Fill(float amount) {
	m_xFullness.GetCurrentModifycationType().Add(amount);

	UpdateInfo();
}

TOptional<float> AConsumeSource::TryConsumeOut(float amount, const APig* consumer, const UConsumeSpotComponent* spot) {
	if(!m_vConsumeSpots.Contains(spot)) return NullOpt;

	auto spotController = GetOwnerFarm()->GetConsumeSpotsController();

	if(!spotController->IsOccupiedBy(spot, consumer)) return NullOpt;

	auto current = m_xFullness.GetCurrent();
	if(FMath::IsNearlyZero(current)) return NullOpt;

	if(amount > current) {
		amount = current;
	}

	m_xFullness.GetCurrentModifycationType().Add(-amount);

	UpdateInfo();

	return amount;
}