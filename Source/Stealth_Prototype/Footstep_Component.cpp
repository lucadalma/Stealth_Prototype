// Fill out your copyright notice in the Description page of Project Settings.


#include "Footstep_Component.h"
#include "Stealth_PrototypeCharacter.h"
#include "Stealth_PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Perception/AISense_Hearing.h"

//Codice per attivare in consolo il Debug del footstep
static TAutoConsoleVariable<int32> CVarShowFootsteps(
	TEXT("ShowDebugFootsteps"),
	0,
	TEXT("Draws debug info about footsteps")
	TEXT(" 0: off/n")
	TEXT(" 1: on/n"),
	ECVF_Cheat);

//Costruttore
UFootstep_Component::UFootstep_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}
//Begin Play
void UFootstep_Component::BeginPlay()
{
	Super::BeginPlay();

}

//Funzione per gestire il passo
void UFootstep_Component::HandleFootstep(EFoot Foot, float Loudness, float MaxRange)
{
	//Ottendo il player
	if (AStealth_PrototypeCharacter* Character = Cast<AStealth_PrototypeCharacter>(GetOwner()))
	{

		const int32 DebugShowFootsteps = CVarShowFootsteps.GetValueOnAnyThread();

		//Ottengo la Skeletal Mesh del player
		if (USkeletalMeshComponent* Mesh = Character->GetMesh())
		{
			FHitResult HitResult;
			//Ottengo il Vector o del piede sinistro o destro
			const FVector SocketLocation = Mesh->GetSocketLocation(Foot == EFoot::Left ? LeftFootSocket : RightFootSocket);
			//Offset di 20 verso l'alto
			const FVector Location = SocketLocation + FVector::UpVector * 20;

			FCollisionQueryParams QueryParams;
			//ritorno il materiale del suolo
			QueryParams.bReturnPhysicalMaterial = true;
			//Ignoro il player
			QueryParams.AddIgnoredActor(Character);

			//LineTrace verso il basso per capire cosa c'è sotto al player
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Location, Location + FVector::UpVector * -50.f, ECollisionChannel::ECC_WorldStatic, QueryParams))
			{
				//Controllo se ha colpito un oggetto fisico
				if (HitResult.bBlockingHit)
				{
					//Ottendo il PhysicalMaterial del suolo
					if (HitResult.PhysMaterial.Get())
					{
						//Casto alla mia classe PhysicalMaterial
						UStealth_PhysicalMaterial* PhysicalMaterial = Cast<UStealth_PhysicalMaterial>(HitResult.PhysMaterial.Get());
						//check
						if (PhysicalMaterial)
						{
							//Avvio il suono in quella posizione
							UGameplayStatics::PlaySoundAtLocation(this, PhysicalMaterial->FootStepSound, Location, 1.f);
							//Check
							if (Mesh && Mesh->GetOwner())
							{
								AActor* Instigator = Mesh->GetOwner();
								//Genero un suono udibile dall'AI
								UAISense_Hearing::ReportNoiseEvent(Instigator->GetWorld(), Instigator->GetActorLocation(), Loudness, Instigator, MaxRange);
							}
							//Debug del passo
							if (DebugShowFootsteps > 0)
							{
								DrawDebugString(GetWorld(), Location, PhysicalMaterial->GetName(), nullptr, FColor::White, 4.f);
							}
						}

					}

					if (DebugShowFootsteps > 0)
					{
						DrawDebugSphere(GetWorld(), Location, 16, 16, FColor::Red, false, 4.f);
					}

				}
				else
				{

					if (DebugShowFootsteps > 0)
					{
						DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Red, false, 4, 0, 1);
					}

				}

			}
			else
			{
				if (DebugShowFootsteps > 0)
				{
					DrawDebugLine(GetWorld(), Location, Location + FVector::UpVector * -50.f, FColor::Red, false, 4, 0, 1);
					DrawDebugSphere(GetWorld(), Location, 16, 16, FColor::Red, false, 4.f);
				}
			}
		}
	}
}

