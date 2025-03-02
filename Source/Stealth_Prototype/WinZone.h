// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Stealth_PrototypeGameMode.h"
#include "WinZone.generated.h"

UCLASS()
class STEALTH_PROTOTYPE_API AWinZone : public AActor
{
	GENERATED_BODY()
	
public:	
	//Costruttore
	AWinZone();

protected:
	virtual void BeginPlay() override;


private:
	//Zona di trigger
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerZone;

private:
	//Funzione OnOverlapBegin
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
