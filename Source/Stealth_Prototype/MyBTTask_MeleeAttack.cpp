// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MeleeAttack.h"
#include "CombatInterface.h"
#include "StealthAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

//Costruttore
UMyBTTask_MeleeAttack::UMyBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

//Execute Task
EBTNodeResult::Type UMyBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Fuori dal range
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	//Controllo se il player è fuori dal range
	if (OutOfRange)
	{

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	//Ottengo AI
	auto const* const AI = OwnerComp.GetAIOwner();
	//Ottendo Enemy
	auto* const Enemy = Cast<AEnemy>(AI->GetPawn());

	//Controllo se ha l'interfacci di combsattimento
	if (auto* const icombat = Cast<ICombatInterface>(Enemy))
	{
		//Esegui MeleeAttack
		if (MontegeHasFinished(Enemy))
		{
			icombat->Execute_MeleeAttack(Enemy);
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

//Funzione bool che controlla se l'animazione è terminata
bool UMyBTTask_MeleeAttack::MontegeHasFinished(AEnemy* const enemy)
{
	return enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(enemy->GetMontage());
}
