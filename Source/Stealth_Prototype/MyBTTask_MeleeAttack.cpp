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

UMyBTTask_MeleeAttack::UMyBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMyBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	auto const* const cont = OwnerComp.GetAIOwner();
	auto* const enemy = Cast<AEnemy>(cont->GetPawn());

	if (auto* const icombat = Cast<ICombatInterface>(enemy))
	{
		if (MontegeHasFinished(enemy))
		{
			icombat->Execute_MeleeAttack(enemy);
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UMyBTTask_MeleeAttack::MontegeHasFinished(AEnemy* const enemy)
{
	return enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(enemy->GetMontage());
}
