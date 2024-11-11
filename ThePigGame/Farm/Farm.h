#pragma once

#include "CoreMinimal.h"
#include "Farm.generated.h"

class UConsumeSpotsController;
class UPigsController;
class USleepingPigsController;
class UTroughsController;

UCLASS()
class THEPIGGAME_API AFarm : public AActor {
	GENERATED_BODY()

	public:	
	AFarm();

	public:
	UPigsController* GetPigsController();
	USleepingPigsController* GetSleepingPigsController();
	UConsumeSpotsController* GetConsumeSpotsController();

	public:
	UStaticMeshComponent* GetSleepingArea();

	protected:
	void BeginPlay() override;

	private:
	void InitControllers();

	protected:
	UPROPERTY()
	UPigsController* m_pPigsController = nullptr;

	UPROPERTY()
	USleepingPigsController* m_pSleepingPigsController = nullptr;

	UPROPERTY()
	UConsumeSpotsController* m_pConsumeSpotsController = nullptr;

	protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* m_pSleepingArea = nullptr;
};