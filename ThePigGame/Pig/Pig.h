// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pig.generated.h"

class UVisualInfoController;
class UAnimInstanceController;
class USupremePropertyController;
class USkeletalMeshComponent;
class UTextRenderComponent;
class APigAIController;
class AFarm;
class UPigStateMachine;
class UPigAnimInstance;
class UTaskDispatcher;
class UMovementController;
class UPigInitData;

UCLASS(Blueprintable)
class THEPIGGAME_API APig : public ACharacter {
	GENERATED_BODY()

	public:
	APig();

	virtual void Tick(float DeltaTime) override;

	AFarm* GetOwnerFarm();
	const UPigInitData* GetInitData();
	USupremePropertyController* GetPropertyController();
	UPigStateMachine* GetPigStateMachine();
	UTaskDispatcher* GetTaskDispatcher();
	UMovementController* GetMovementController();
	APigAIController* GetPigAIController();
	UAnimInstanceController* GetAnimInstanceController();
	UPigAnimInstance* GetPigAnimInstance();
	UTextRenderComponent* GetPigInfoComponent();

	protected:
	virtual void BeginPlay() override;

	private:
	void CreateObjects();
	void InitObjects();
	void AfterInitObjects();

	protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, NoClear)
	UPigInitData* m_pInitData = nullptr;

	private:
	UPROPERTY()
	USupremePropertyController* m_pPropertyController = nullptr;

	UPROPERTY()
	UPigStateMachine* m_pStateMachine = nullptr;

	UPROPERTY()
	UTaskDispatcher* m_pTaskDispatcher = nullptr;

	UPROPERTY()
	UMovementController* m_pMovementController = nullptr;

	UPROPERTY()
	UAnimInstanceController* m_pAnimInstanceController = nullptr;

	UPROPERTY()
	UVisualInfoController* m_pVisualInfoController = nullptr;

	UPROPERTY()
	UTextRenderComponent* PigInfo = nullptr;
};