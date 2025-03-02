// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

//Funzione per Restartare la partita
void AMyPlayerController::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

//Funzione per capire quale Widget mostrare a fine partita
void AMyPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	//Se il player ha vinto
	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	//Se il player ha perso
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}
	//Restarto la partita
	GetWorldTimerManager().SetTimer(RestartTimer, this, &AMyPlayerController::RestartGame, RestartDelay, false);

}
