#include "VisualInfoController.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThePigGame/Levels/MainLevel/GameMainPlayerController.h"
#include "ThePigGame/Levels/MainLevel/MainScreenGameMode.h"
#include "ThePigGame/Pig/PigProperties/PigPropertyType.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"

void UVisualInfoController::Tick(float deltaTime) {
	FillInfo();
}

void UVisualInfoController::FillInfo() {
	FString str(UTF8_TO_TCHAR("Current age = "));
	auto propertyController = GetPropertyController();

	auto currentAge = propertyController->GetProperty<EPigPropertyType::Age>()->GetCurrent();
	auto gameMode = Cast<AMainScreenGameMode>(GetWorld()->GetAuthGameMode());
	str += FString::SanitizeFloat(currentAge / gameMode->GetOneYearInSeconds());
	str += " years";

	str += "\nBellyful: ";
	str += FString::SanitizeFloat(propertyController->GetProperty<EPigPropertyType::Bellyful>()->GetCurrent());

	str += "\nThirst: ";
	str += FString::SanitizeFloat(propertyController->GetProperty<EPigPropertyType::Hydrated>()->GetCurrent());

	auto currentWeight = propertyController->GetProperty<EPigPropertyType::Weight>()->GetCurrent();
	auto currentCriticalWeight = propertyController->GetProperty<EPigPropertyType::CriticalWeight>()->GetCurrent();
	auto currentMaxWeight = propertyController->GetProperty<EPigPropertyType::MaxWeight>()->GetCurrent();

	str += "\nCurrent weight = ";
	str += FString::SanitizeFloat(currentWeight);
	str += " kg ";
	{
		auto weight = 100.f * (currentWeight - currentCriticalWeight) / (currentMaxWeight - currentCriticalWeight);
		str += FString::SanitizeFloat(weight);
	}
	str += " %";
	str += "\nCritical = ";
	str += FString::SanitizeFloat(currentCriticalWeight);
	str += "\nMax possible = ";
	str += FString::SanitizeFloat(currentMaxWeight);

	str += "\nCurrent bellyful = ";
	str += FString::SanitizeFloat(propertyController->GetProperty<EPigPropertyType::Bellyful>()->GetCurrent());
	str += "\n";

	str += "Current energy = ";
	str += FString::SanitizeFloat(propertyController->GetProperty<EPigPropertyType::Energy>()->GetCurrent());

	GetPig()->GetPigInfoComponent()->SetText(FText::FromString(str));

	auto plc = Cast<AGameMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	plc->SetPigInfo(str);
}