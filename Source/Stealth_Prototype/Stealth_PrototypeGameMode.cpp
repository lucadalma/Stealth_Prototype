// Copyright Epic Games, Inc. All Rights Reserved.

#include "Stealth_PrototypeGameMode.h"
#include "Stealth_PrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"

AStealth_PrototypeGameMode::AStealth_PrototypeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AStealth_PrototypeGameMode::PawnKilled(APawn* PawnKilled)
{

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}
}

void AStealth_PrototypeGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
