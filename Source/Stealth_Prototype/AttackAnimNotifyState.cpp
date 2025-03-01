// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "Enemy.h"

//Inizio Attacco Nemico
void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//Controllo il MeshComp
	if (MeshComp && MeshComp->GetOwner()) 
	{
		//Ottengo l'enemy
		if (AEnemy* const enemy = Cast<AEnemy>(MeshComp->GetOwner())) 
		{
			//Chiamo funzione AttackStart()
			enemy->AttackStart();
		}
	}
}

//Fine Attacco nemico
void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Controllo il MeshComp
	if (MeshComp && MeshComp->GetOwner())
	{
		//Ottengo l'enemy
		if (AEnemy* const enemy = Cast<AEnemy>(MeshComp->GetOwner()))
		{
			//Chiamo funzione AttackStart()
			enemy->AttackEnd();
		}
	}
}
