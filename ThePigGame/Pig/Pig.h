// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PigProperties/PigProperties.h"
#include "PigInitData.h"
#include "../Utils/EventHandler/TEventHandler.h"
#include "PigEvent.h"
#include "Tasks/TaskType.h"
#include "Pig.generated.h"

class USkeletalMeshComponent;
class UTextRenderComponent;
class APigAIController;
class UEatingSpot;
class AFarm;
class UPigStateMachine;
class UPigAnimInstance;
class UTaskDispatcher;

UCLASS(Blueprintable)
class THEPIGGAME_API APig : public ACharacter, public IPropertyTickProvider, public TEventHandler<EPigEvent> {
	GENERATED_BODY()

	public:
	APig();

	public:	
	virtual void Tick(float DeltaTime) override;
	
	protected:
	virtual void BeginPlay() override;

	protected:
	void Init();

	public:
	AFarm* GetOwnerFarm();

	public:
	void SetPigAIController(APigAIController* AIContoller);
	APigAIController* GetPigAIController();

	protected:
	void SubscribeOnAIController();

	public:
	bool CanWeightTick();

	protected:
	void CheckIfAdult();

	protected:
	// Sets scale
	void SetPigScale();

	protected:
	// Fatness. Sets mesh's morph targets
	void SetMeshMorphs();

	protected:
	void CheckBellyfulLevel();
	void CheckEnergyLevel();

	protected:
	void FillPigInfo();

	// everything about state machine

	public:
	UPigStateMachine* GetPigStateMachine();

	protected:
	void CreateStateMachine();

	protected:
	void SubscribeOnStatesEvents();

	protected:
	void SubscribeOnSleepingState();
	void SubscribeOnEatingState();

	// animations
	public:
	UPigAnimInstance* GetPigAnimInstance();

	protected:
	void SubscribeOnAnimations();

	// task dispatcher

	public:
	UTaskDispatcher* GetTaskDispatcher();

	protected:
	void CreateTaskDispatcher();

	public:
	void AddTask(ETaskType taskType);

	public:
	void SetWaitingForEatingSpot(bool isWaiting);


	protected:
	Age m_xAge;

	protected:
	Scale m_xScale;

	protected:
	Bellyful m_xBellyful;

	protected:
	MaxWeight m_xMaxWeight;
	CriticalWeight m_xCriticalWeight;
	Weight m_xWeight;

	protected:
	Energy m_xEnergy;

	protected:
	Morph m_xMorph;	

	protected:
	float m_fMaxAgeAtSeconds = 0.f;
	float m_fMaxSizesAtSeconds = 0.f;
	float m_fWeightDeltaPerTick = 0.f;
	float m_fAgeWhenAdultSeconds = 0.f;


	protected:
	UPROPERTY()
	FPigInitData m_xInitData;

	protected:
	UPROPERTY()
	UPigStateMachine* m_pStateMachine = nullptr;

	protected:
	UPROPERTY()
	APigAIController* m_pPigAIController = nullptr;

	protected:
	UPROPERTY()
	UTaskDispatcher* m_pTaskDispatcher = nullptr;


	protected:
	UPROPERTY()
	UTextRenderComponent* PigInfo = nullptr;

	protected:
	bool m_bIsAdult = false;

	protected:
	AFarm* m_pOwnerFarm = nullptr;

	protected:
	UPROPERTY()
	USkeletalMesh* m_xAdultMesh;

	protected:
	UPROPERTY()
	UClass* m_xAdultAnimBlueprint;

	protected:
	bool m_bIsWaitingForEatingSpot = false;


};
