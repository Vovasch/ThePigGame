// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SleepingArea.generated.h"

class APig;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEPIGGAME_API USleepingArea : public UStaticMeshComponent {
	GENERATED_BODY()

	public:		
	USleepingArea();

	public:
	void OnPigStartedSleeping(APig* pig);
	void OnPigEndedSleeping(APig* pig);

	public:
	APig* GetAnySleepingPig() const;

	protected:
	virtual void BeginPlay() override;

	public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	protected:
	TSet<APig*> m_vSleepingPigs;
	
};
