// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Attack.h"
#include "StealthAIController.h"
#include "Stealth_PrototypeEnemy.h"

UMyBTTask_Attack::UMyBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UMyBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	AStealth_PrototypeEnemy* Character = Cast<AStealth_PrototypeEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr)
		return EBTNodeResult::Failed;

	//Character->Shoot();

	return EBTNodeResult::Succeeded;
}
