#include "PG_GameInstance.h"
#include "AssetStorage.h"

UPG_GameInstance::UPG_GameInstance() {
	m_pAssetStorage = CreateDefaultSubobject<UAssetStorage>("AssetStorage");
}

const UAssetStorage* UPG_GameInstance::GetAssetStorage() {
	return m_pAssetStorage;
}
