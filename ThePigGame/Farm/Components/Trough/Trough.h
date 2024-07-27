// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EatingSpot.h"
#include "ThePigGame/Utils/Property/Property.h"
#include "Trough.generated.h"

class UTextRenderComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class THEPIGGAME_API UTroughPrototype : public UObject {

	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere)
	float m_fCapacity = 100.f;

	UPROPERTY(EditAnywhere)
	float m_fStaringAmount = 50.f;
};

using TTroughtFillness = TPropertyBase<TMinMaxStatic, TNoTickType, TAllowedCurrentModification>;

UCLASS()
class THEPIGGAME_API ATrough : public AActor {
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	ATrough();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:
	virtual void OnConstruction(const FTransform& Transform) override;

	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	public:
	void Fill(float amount);

	float TryEatOut(float amount);

	public:
	UEatingSpot* GetAvailableEatingSpot();

	public:
	TArray<UEatingSpot*>& GetAllEatingSpots();


	protected:
	void UpdateInfo();

	public:
	UPROPERTY(EditAnywhere)
	UTroughPrototype* m_pPrototype = nullptr;

	UPROPERTY(EditAnyWhere)
	UStaticMeshComponent* TroughMesh = nullptr;

	public:
	UPROPERTY(EditAnyWhere)
	UTextRenderComponent* Info = nullptr;

	public:
	UPROPERTY()
	TArray<UEatingSpot*> m_vEatingSpots;

	TTroughtFillness m_xFullness;

};
