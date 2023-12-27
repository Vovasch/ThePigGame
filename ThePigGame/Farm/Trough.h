// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EatingSpot.h"
#include "Trough.generated.h"

class UTextRenderComponent;
class UStaticMeshComponent;


UCLASS()
class THEPIGGAME_API ATrough : public AActor {
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	ATrough();

	public:
	virtual void OnConstruction(const FTransform& Transform) override;

	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	public:
	void Fill(float amount);

	public:
	UEatingSpot* GetAvailableEatingSpot();

	public:
	TArray<UEatingSpot*>& GetAllEatingSpots();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	protected:
	void UpdateInfo();

	public:
	UPROPERTY(EditAnyWhere)
	UStaticMeshComponent* TroughMesh = nullptr;

	public:
	UPROPERTY(EditAnyWhere)
	UTextRenderComponent* Info = nullptr;

	public:
	UPROPERTY()
	TArray<UEatingSpot*> m_vEatingSpots;

	public:
	UPROPERTY(EditAnyWhere)
	float m_fCapacity = 100.f;

	protected:
	float m_fCurrentFillnes = 0.f;

};
