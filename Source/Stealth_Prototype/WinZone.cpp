// Fill out your copyright notice in the Description page of Project Settings.


#include "WinZone.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Stealth_PrototypeCharacter.h"

//Costruttore
AWinZone::AWinZone()
{
	PrimaryActorTick.bCanEverTick = false;
	//Aggiungo una TriggerZone
	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	RootComponent = TriggerZone;
	//Imposto le collisioni
	TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerZone->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//Se Begiun Overlap chiama la funzione OnOverlapBegin
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AWinZone::OnOverlapBegin);

}

void AWinZone::BeginPlay()
{
	Super::BeginPlay();
}

//Funzione se il player entra nella trigger zone vince
void AWinZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Controlla se è il player
	if (OtherActor && OtherActor->IsA(AStealth_PrototypeCharacter::StaticClass()))
	{
		//Ottengo la GameMode
		AStealth_PrototypeGameMode* GameMode = Cast<AStealth_PrototypeGameMode>(GetWorld()->GetAuthGameMode());
		//Check
		if (GameMode)
		{
			//Il player vince
			GameMode->EndGame(true);
		}
	}

}

