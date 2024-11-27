#pragma once

#include "CoreMinimal.h"
#include "ConsumeSourceType.h"
#include "ThePigGame/Utils/Property/Property.h"
#include "ConsumeSource.generated.h"

class APig;
class AFarm;
class UConsumeSourcePrototype;
class UConsumeSpotComponent;
class UTextRenderComponent;

/// Designed to be created inside of AFarm actor
///	Though using UChildActorComponent

UCLASS()
class AConsumeSource : public AActor {
	GENERATED_BODY()

	using TConsumeSourceFullness = TPropertyBase<TMinMaxStatic, TNoTickType, TAllowedCurrentModification>;

	public:
	AConsumeSource();

	public:
	virtual EConsumeSourceType GetConsumeType() PURE_VIRTUAL("Not implemented", return EConsumeSourceType::Size;)

	public:
	void Fill(float amount);
	TOptional<float> TryConsumeOut(float amount, const APig* consumer, const UConsumeSpotComponent* spot);

	public:
	AFarm* GetOwnerFarm();

	protected:
	void BeginPlay() override;
	void InitProperties();
	void InitConsumeSpots();

	protected:
	void UpdateInfo();

	protected:
	UPROPERTY(EditAnywhere)
	UConsumeSourcePrototype* m_pPrototype = nullptr;

	UPROPERTY(EditAnyWhere)
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnyWhere)
	UTextRenderComponent* Info = nullptr;

	TConsumeSourceFullness m_xFullness;

	private:
	// no need for UProperty, these are actor components.
	TArray<UConsumeSpotComponent*> m_vConsumeSpots;

};