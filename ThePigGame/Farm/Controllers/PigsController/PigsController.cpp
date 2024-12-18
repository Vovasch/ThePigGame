#include "PigsController.h"
#include "ThePigGame/Farm/Farm.h"
#include "ThePigGame/Farm/Controllers/SleepingPigsController/SleepingPigsController.h"
#include "ThePigGame/Infrastructure/AssetStorage.h"
#include "ThePigGame/Infrastructure/PG_GameInstance.h"
#include "ThePigGame/Pig/Pig.h"

void UPigsController::BeginPlay() {
	SpawnTestPigs();
}

void UPigsController::SpawnTestPigs() {
	FVector origin;
	FVector boxExtent;
	GetFarm()->GetActorBounds(false, origin, boxExtent);

	auto uclass = Cast<UPG_GameInstance>(GetWorld()->GetGameInstance())->GetAssetStorage()->PigBP;

	origin.Z = 88.f;
	for(int i = 0; i < 1; i++) {
		auto pig = GetWorld()->SpawnActor<APig>(uclass, origin, FRotator::ZeroRotator);
		if(!pig) continue;
		origin.X += 70;
		AddPigToFarm(pig);
	}
}

void UPigsController::AddPigToFarm(APig* pig) {
	m_vPigs.Add(pig);
	pig->SetOwner(GetFarm());
	GetFarm()->GetSleepingPigsController()->RegisterPig(pig);
}
