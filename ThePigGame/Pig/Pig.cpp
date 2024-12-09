#include "Pig.h"
#include "Components/TextRenderComponent.h"
#include "PigAI/PigAIController.h"
#include "../Farm/Farm.h"
#include "PigStateMachine/PigStateMachine.h"
#include "AnimInstance/AnimInstanceController.h"
#include "AnimInstance/PigAnimInstance.h"
#include "Movement/MovementController.h"
#include "PropertyControllers/SupremePropertyController/SupremePropertyController.h"
#include "TasksInfrastructure/TaskDispatcher/TaskDispatcher.h"
#include "VisualInfoController/VisualInfoController.h"

APig::APig() {

	PrimaryActorTick.bCanEverTick = true;

	PigInfo = CreateDefaultSubobject<UTextRenderComponent, UTextRenderComponent>("PigInfo");
	PigInfo->SetupAttachment(GetRootComponent());
	PigInfo->bHiddenInGame = true;

	CreateObjects();
}

void APig::BeginPlay() {
	Super::BeginPlay();

	InitObjects();

	AfterInitObjects();
}

void APig::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_pPropertyController->Tick(DeltaTime);
	m_pStateMachine->Tick(DeltaTime);
	m_pTaskDispatcher->Tick(DeltaTime);
	m_pMovementController->Tick(DeltaTime);
	m_pVisualInfoController->Tick(DeltaTime);
}

void APig::CreateObjects() {
	m_pPropertyController = CreateDefaultSubobject<USupremePropertyController>(TEXT("PropertyController"));
	m_pStateMachine = CreateDefaultSubobject<UPigStateMachine>(TEXT("StateMachine"));
	m_pTaskDispatcher = CreateDefaultSubobject<UTaskDispatcher>(TEXT("TaskDispatcher"));
	m_pMovementController = CreateDefaultSubobject<UMovementController>(TEXT("MovementController"));
	m_pAnimInstanceController = CreateDefaultSubobject<UAnimInstanceController>(TEXT("AnimInstanceController"));
	m_pVisualInfoController = CreateDefaultSubobject<UVisualInfoController>(TEXT("VisualInfoController"));
}

void APig::InitObjects() {
	m_pPropertyController->Init(this);
	m_pStateMachine->Init(this);
	m_pTaskDispatcher->Init(this);
	m_pMovementController->Init(this);
	m_pAnimInstanceController->Init(this);
	m_pVisualInfoController->Init(this);

	GetPigAnimInstance()->Init(this);
}

void APig::AfterInitObjects() {
	m_pStateMachine->AfterInit();
}

AFarm* APig::GetOwnerFarm() {
	return Cast<AFarm>(GetOwner());
}

const UPigInitData* APig::GetInitData() {
	return m_pInitData;
}

USupremePropertyController* APig::GetPropertyController() {
	return m_pPropertyController;
}

UPigStateMachine* APig::GetPigStateMachine() {
	return m_pStateMachine;
}

UTaskDispatcher* APig::GetTaskDispatcher() {
	return m_pTaskDispatcher;
}

UMovementController* APig::GetMovementController() {
	return m_pMovementController;
}

APigAIController* APig::GetPigAIController() {
	return Cast<APigAIController>(Controller);
}

UAnimInstanceController* APig::GetAnimInstanceController() {
	return m_pAnimInstanceController;
}

UPigAnimInstance* APig::GetPigAnimInstance() {
	return Cast<UPigAnimInstance>(GetMesh()->GetAnimInstance());
}

UTextRenderComponent* APig::GetPigInfoComponent() {
	return PigInfo;
}
