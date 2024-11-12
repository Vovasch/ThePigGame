#include "MovementController.h"
#include "../Pig.h"
#include "RotationController/RotationController.h"
#include "ThePigGame/Pig/PigInitData.h"
#include "ThePigGame/Pig/PropertyControllers/PropertySubControllers/AgeController/AgeController.h"
#include "ThePigGame/Pig/PropertyControllers/SupremePropertyController/SupremePropertyController.h"
#include "WalkingController/WalkingController.h"

DEFINE_LOG_CATEGORY_STATIC(MovementControllerLog, Log, All)


/*void UMovementController::BeginPlay() {
	
	FMemMark Mark(FMemStack::Get());

	m_xAnimation = Cast<UAnimSequence>(FSoftObjectPath(TEXT("/Game/Animalia/Pig_Y/Animations/Trans_TurnR90.Trans_TurnR90")).TryLoad());

	
	FCompactPose m_xOutPose; // This will be set when extracting the pose
	FBlendedCurve m_xOutCurve;
	FStackCustomAttributes m_xOutAttr;
	m_xOutPose.SetBoneContainer(&GetPig()->GetPigAnimInstance()->GetRequiredBones());
	FAnimationPoseData m_xOutData(m_xOutPose, m_xOutCurve, m_xOutAttr);

	auto framesCount = m_xAnimation->GetNumberOfFrames();

	for(int32 i = 0; i<framesCount; ++i) {
		m_xAnimation->GetAnimationPose(m_xOutData, FAnimExtractContext(m_xAnimation->GetTimeAtFrame(i)));
		m_vTrasformAtFrame.Add(m_xOutData.GetPose().GetBones()[0]);		
	}
	
}*/

UMovementController::UMovementController() {
	m_pRotationController = CreateDefaultSubobject<URotationController>(TEXT("RotationController"));
}

void UMovementController::Init(APig* pig) {
	ICachedPigDataUser::Init(pig);
	
	m_pWalkingController = pig->GetWorld()->SpawnActor<AWalkingController>(GetInitData()->WalkingControllerBP);
	m_pWalkingController->SetActorHiddenInGame(true);
	m_pWalkingController->SetOwner(GetPig());

	m_pWalkingController->Init(pig);
	m_pRotationController->Init(pig);
	m_pRotationController->SetWalkingController(m_pWalkingController);

	auto ageController = GetPropertyController()->GetAgeController();

	if(ageController->GetIsAdult()) {
		InitAdultVelocityData();
	} else {
		InitChildVelocityData();

		ageController->Subscribe(this, EAgeControllerEvent::BecomeAdult, [this]() {
			InitAdultVelocityData();
		});
	}

}

float UMovementController::GetVelocity() {
	return m_fLinearVelocityMagnitude;
}

float UMovementController::GetRotation() {
	return m_fAngularVelocity;
}

void UMovementController::InitChildVelocityData() {
	InitVelocityData(false);
}

void UMovementController::InitAdultVelocityData() {
	InitVelocityData(true);
}

void UMovementController::InitVelocityData(bool isAdult) {
	auto initData = GetPig()->GetInitData();

	m_pWalkingController->InitWalkingData(isAdult ? initData->AdultWalkingData : initData->ChildWalkingData);
	m_pRotationController->InitRotationData(isAdult ? initData->AdultRotationData : initData->ChildRotationData);
}

void UMovementController::MoveTo(const FVector& vec) {
	m_pRotationController->Unsubscribe(this);
	m_pWalkingController->Unsubscribe(this);

	m_pRotationController->Subscribe(this, ERotationControllerEvent::RotationCompleted, [this, vec]() {
		m_pRotationController->Unsubscribe(this);

		m_pWalkingController->Unsubscribe(this);

		
		m_pWalkingController->Subscribe(this, EWalkingControllerEvent::WalkingCompleted, [this]() {
			m_pWalkingController->Unsubscribe(this);
			OnEvent(EMovementControllerEvent::MovementCompleted);
		});

		m_pWalkingController->Subscribe(this, EWalkingControllerEvent::WalkingFailed, [this]() {
			OnEvent(EMovementControllerEvent::MovementFailed);
		});

		m_pWalkingController->WalkTo(vec);
	});

	m_pRotationController->RotateTo(vec);

}

void UMovementController::InterruptMovement() {
	m_pRotationController->Unsubscribe(this);
	m_pWalkingController->Unsubscribe(this);

	m_pWalkingController->InterruptWalking();
}

void UMovementController::Tick(float deltaTime) {
	m_pRotationController->Tick(deltaTime);

	m_fAngularVelocity = m_pRotationController->GetAngularVelocity();

	if(m_pRotationController->IsCurrentlyRotating()) {
		m_fLinearVelocityMagnitude = m_pRotationController->GetLinearVelocityMagnitude();
	} else {
		m_fLinearVelocityMagnitude = m_pWalkingController->GetLinearVelocityMagnitude();
	}
}
