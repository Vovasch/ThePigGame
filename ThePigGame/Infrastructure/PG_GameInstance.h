#pragma once

#include "CoreMinimal.h"
#include "PG_GameInstance.generated.h"

class UAssetStorage;

UCLASS()
class UPG_GameInstance : public UGameInstance {
	GENERATED_BODY()

	public:
	UPG_GameInstance();

	public:
	const UAssetStorage* GetAssetStorage();

	private:
	UPROPERTY(EditAnywhere, NoClear)
	UAssetStorage* m_pAssetStorage = nullptr;
};
