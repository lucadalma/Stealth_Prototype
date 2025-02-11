// Copyright Epic Games, Inc. All Rights Reserved.

#include "Stealth_PrototypeGameMode.h"
#include "Stealth_PrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStealth_PrototypeGameMode::AStealth_PrototypeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
