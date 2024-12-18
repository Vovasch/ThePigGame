#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "ThePigGame/Farm/Components/ConsumeSource/ConsumeSourceType.h"
#include "PigInitData.generated.h"

class UPigAnimInstance;
class AWalkingController;

USTRUCT()
struct FPigWalkingData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SneakSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float SlowWalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float TrotSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 0.0f;

	UPROPERTY(EditAnywhere)
	float SprintSpeed = 0.0f;
};

USTRUCT()
struct FPigRotationData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float WalkDeltaX = 0.f;

	UPROPERTY(EditAnywhere)
	float WalkDeltaY = 0.f;

	UPROPERTY(EditAnywhere)
	float TimeTaken = 0.f;

	UPROPERTY(EditAnywhere)
	float DegreesInTime = 0.f;
};

USTRUCT()
struct FConsumingData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float StartingValue = 100.f;

	UPROPERTY(EditAnyWhere)
	float Min = 0.f;

	UPROPERTY(EditAnyWhere)
	float Max = 100.f;

	UPROPERTY(EditAnyWhere)
	float AmountToWantToConsume = 40.f;

	UPROPERTY(EditAnyWhere)
	float AmountToStopConsume = 90.f;

	UPROPERTY(EditAnywhere)
	float GainDeltaPerTick = 1.f;

	UPROPERTY(EditAnywhere)
	float LoseDeltaPerTick = 0.1f;

	UPROPERTY(EditAnywhere)
	float WaitUntilRetryConsume = 100.f;
};


UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class UPigInitData : public UObject {
	GENERATED_BODY()

	public:

	UPigInitData();

	UPROPERTY(EditAnywhere)
	USkeletalMesh* m_xAdultMesh = nullptr;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UPigAnimInstance> m_xAdultAnimBlueprint = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWalkingController> WalkingControllerBP = nullptr;

	//
	// AGE 
	//

	// At what age in years pig is old enough to be out of business
	UPROPERTY(EditAnyWhere)
	float MaxAgeYears = 18.f;	

	UPROPERTY(EditAnywhere)
	float AgeOfAdultYears = 5.f;

	//
	// SCALE
	//

	UPROPERTY(EditAnywhere)
	float ChildMinScale = 0.8;

	UPROPERTY(EditAnywhere)
	float ChildMaxScale = 1.3;

	UPROPERTY(EditAnywhere)
	float AdultMinScale = 0.5;

	UPROPERTY(EditAnywhere);
	float AdultMaxScale = 1.2;

	UPROPERTY(EditAnyWhere)
	float MaxScale = 3.f;

	UPROPERTY(EditAnyWhere)
	float MinScale = 0.5f;
	
	//
	// Weight. In kg.
	//

	
	UPROPERTY(EditAnywhere)
	float CriticalWeightAtMinAge = 1.f;

	UPROPERTY(EditAnywhere)
	float CriticalWeightAtMaxAge = 120.f;
	
	UPROPERTY(EditAnywhere)
	float MaxWeightAtMinAge = 2.f;

	UPROPERTY(EditAnywhere)
	float MaxWeightAtMaxAge = 300.f;
	
	//
	// Consuming
	//

	UPROPERTY(EditAnywhere, meta = (EditFixedSize))
	TMap<EConsumeSourceType, FConsumingData> ConsumingData;

	//
	// Bellyful
	//

	UPROPERTY(EditAnyWhere)
	float BellyfulLevelToGetFat = 70.f;

	UPROPERTY(EditAnyWhere)
	float BellyfulLevelToGetThin = 20.f;

	//
	// Max Sizes 
	//

	// At what age of pig it will reach it's max scale and max possible weight.
	
	UPROPERTY(EditAnyWhere)
	float MaxSizesAtAgeYears = 8.f;

	//
	// Energy
	//
	
	UPROPERTY(EditAnyWhere)
	float MinEnergy = 0.f;

	UPROPERTY(EditAnyWhere)
	float MaxEnergy = 100.f;

	UPROPERTY(EditAnyWhere)
	float GainignEnergyDelta = 3.f;

	UPROPERTY(EditAnyWhere)
	float LoosingEnergyDelta = -0.1f;

	UPROPERTY(EditAnyWhere)
	float EnergyLevelToWantToSleepLevel = 20.f;

	UPROPERTY(EditAnyWhere)
	float EnergyLevelSleepedEnought = 80.f;

	//
	// Morph
	//

	
	UPROPERTY(EditAnyWhere)
	float MorphTargetMinValue = -1.f;

	UPROPERTY(EditAnyWhere);
	float MorphTargetMaxValue = 1.f;


	//
	// Walking
	//

	UPROPERTY(EditAnyWhere)
	FPigWalkingData ChildWalkingData;

	UPROPERTY(EditAnyWhere)
	FPigWalkingData AdultWalkingData;

	//
	// Rotation
	//

	UPROPERTY(EditAnywhere)
	FPigRotationData ChildRotationData;

	UPROPERTY(EditAnywhere)
	FPigRotationData AdultRotationData;


};