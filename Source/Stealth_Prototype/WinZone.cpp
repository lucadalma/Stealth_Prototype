// Fill out your copyright notice in the Description page of Project Settings.


#include "WinZone.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Stealth_PrototypeCharacter.h"

// Sets default values
AWinZone::AWinZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
    RootComponent = TriggerZone;

    TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerZone->SetCollisionObjectType(ECC_WorldDynamic);
    TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AWinZone::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AWinZone::BeginPlay()
{
    Super::BeginPlay();
}


void AWinZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(AStealth_PrototypeCharacter::StaticClass()))  // Controlla se è il player
    {
        AStealth_PrototypeGameMode* GameMode = Cast<AStealth_PrototypeGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->EndGame(true);
        }
    }

}

