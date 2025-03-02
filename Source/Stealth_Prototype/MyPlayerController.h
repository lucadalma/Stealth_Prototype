// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	//Variabili dei Widget
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> LoseScreenClass;
	//Delay del restart
	float RestartDelay = 5.f;
	//Time Handle
	FTimerHandle RestartTimer;

public:
	//Funzione di fine partita
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
	//Restart partita
	void RestartGame();
};
