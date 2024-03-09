#pragma once

#include "CoreMinimal.h"
#include "PigInitData.generated.h"

USTRUCT()
struct FPigRotationData {
	GENERATED_BODY();

	float WalkDeltaX = 0.f;
	float WalkDeltaY = 0.f;
	float TimeTaken = 0.f;
	float DegreesInTime = 0.f;
};



USTRUCT()
struct FPigInitData {
	GENERATED_BODY();

	FPigInitData();

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
	// Movement
	//

	// Child

	UPROPERTY(EditAnywhere)
	float ChildSneakSpeed = 7.43f;

	UPROPERTY(EditAnywhere)
	float ChildSlowWalkSpeed = 22.44f;

	UPROPERTY(EditAnywhere)
	float ChildWalkSpeed = 31.07f;

	UPROPERTY(EditAnywhere)
	float ChildTrotSpeed = 133.6f;

	UPROPERTY(EditAnywhere)
	float ChildRunSpeed = 225.f;

	UPROPERTY(EditAnywhere)
	float ChildSprintSpeed = 418.125f;

	// Adult

	UPROPERTY(EditAnywhere)
	float AdultSneakSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	float AdultSlowWalkSpeed = 51.9f;

	UPROPERTY(EditAnywhere)
	float AdultWalkSpeed = 72.04f;

	UPROPERTY(EditAnywhere)
	float AdultTrotSpeed = 237.61f;

	UPROPERTY(EditAnywhere)
	float AdultRunSpeed = 514.05f;

	UPROPERTY(EditAnywhere)
	float AdultSprintSpeed = 820.88f;

	//
	// Rotation
	//

	FPigRotationData ChildRotationData;
	FPigRotationData AdultRotationData;


};