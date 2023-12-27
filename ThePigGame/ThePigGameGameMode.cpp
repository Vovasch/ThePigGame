// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThePigGameGameMode.h"
#include "ThePigGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThePigGameGameMode::AThePigGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
