// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "Enemy.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner()) 
	{
		if (AEnemy* const enemy = Cast<AEnemy>(MeshComp->GetOwner())) 
		{
			enemy->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AEnemy* const enemy = Cast<AEnemy>(MeshComp->GetOwner()))
		{
			enemy->AttackEnd();
		}
	}
}
