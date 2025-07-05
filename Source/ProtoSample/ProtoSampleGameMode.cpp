// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProtoSampleGameMode.h"
#include "ProtoSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProtoSampleGameMode::AProtoSampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
