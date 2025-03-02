// Copyright Epic Games, Inc. All Rights Reserved.

#include "Stealth_PrototypeGameMode.h"
#include "Stealth_PrototypeCharacter.h"
#include "MyPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
//Costruttore
AStealth_PrototypeGameMode::AStealth_PrototypeGameMode()
{
	//Imposta il Pawn predefinito
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

//Funzione che mi ritorna false se il player ha perso
void AStealth_PrototypeGameMode::PawnKilled(APawn* PawnKilled)
{
	EndGame(false);
}

//Funzione di fine partita
void AStealth_PrototypeGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
		Controller->SetIgnoreMoveInput(true);
		Controller->SetIgnoreLookInput(true);
	}
}
