// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Step.h"
#include "Footstep_Component.h"

void UAnimNotify_Step::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	check(MeshComp);

	AStealth_PrototypeCharacter* Character = MeshComp ? Cast<AStealth_PrototypeCharacter>(MeshComp->GetOwner()) : nullptr;

	if (Character)
	{
		if (UFootstep_Component* FootstepsComp = Character->GetFootstepComponent())
		{
			FootstepsComp->HandleFootstep(Foot);
		}
	}
}