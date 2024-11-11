// Fill out your copyright notice in the Description page of Project Settings.


#include "PigAIController.h"
#include "Navigation/CrowdFollowingComponent.h"

DEFINE_LOG_CATEGORY_STATIC(AIControllerLog, Log, All)


APigAIController::APigAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) {

}