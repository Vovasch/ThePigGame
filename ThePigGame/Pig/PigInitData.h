#pragma once

#include "CoreMinimal.h"
#include "PigInitData.generated.h"

USTRUCT()
struct FPigInitData {
	GENERATED_BODY();

	// no need
	// Same as MaxAgeYears, but only in seconds
	//float MaxAgeSeconds = 0.f;
	// Same as MaxSizesAtAgeYears, but in seconds.
	//float MaxSizesAtSeconds = 0.f;

	// How much weight of pig can change per tick
	//float WeightDeltaPerTick = 0.f;

	//
	// AGE 
	//

	
	// At what age in years pig is old enough to be out of business
	UPROPERTY(EditAnyWhere)
	float MaxAgeYears = 18.f;	

	UPROPERTY(EditAnywhere)
	float AgeOfAdultYears = 3.f;

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
	// Bellyful
	//

	
	UPROPERTY(EditAnyWhere)
	float MinBellyful = 0.f;

	UPROPERTY(EditAnyWhere)
	float MaxBellyful = 100.f;

	UPROPERTY(EditAnyWhere)
	float BellyfulLevelToGetFat = 70.f;

	UPROPERTY(EditAnyWhere)
	float BellyfulLevelToGetThin = 20.f;

	UPROPERTY(EditAnyWhere)
	float BellyfulLevelToWantToEat = 40.f;

	UPROPERTY(EditAnyWhere)
	float BellyfulLevelToStopEating = 90.f;

	//
	// Max Sizes 
	//

	// At what age of pig it will reach it's max scale and max weight.
	
	UPROPERTY(EditAnyWhere)
	float MaxSizesAtAgeYears = 5.f;

	

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

};