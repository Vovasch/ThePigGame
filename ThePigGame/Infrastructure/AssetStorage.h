#pragma once

#include "CoreMinimal.h"
#include "AssetStorage.generated.h"

class APig;

UCLASS(DefaultToInstanced, EditInlineNew)
class UAssetStorage : public UObject {
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APig> PigBP = nullptr;
};