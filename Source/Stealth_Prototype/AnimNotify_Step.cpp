// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Step.h"
#include "Footstep_Component.h"
#include "GameFramework/Actor.h"
#include "Perception/AISense_Hearing.h"

//Funzione Notify per lo Step del player
void UAnimNotify_Step::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	//Verifico che il MeshComp non sia nullo
	check(MeshComp);

	//Ottengo il player dalla mesh
	AStealth_PrototypeCharacter* Character = MeshComp ? Cast<AStealth_PrototypeCharacter>(MeshComp->GetOwner()) : nullptr;

	//Se trovo player
	if (Character)
	{
		///Ottengo il componente dei passi del player
		if (UFootstep_Component* FootstepsComp = Character->GetFootstepComponent())
		{
			//Gestisco il passo
			FootstepsComp->HandleFootstep(Foot, Loudness, MaxRange);
		}
	}
}