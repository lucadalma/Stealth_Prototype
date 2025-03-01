// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Attack.h"
#include "StealthAIController.h"
#include "Stealth_PrototypeEnemy.h"
//Costruttore
UMyBTTask_Attack::UMyBTTask_Attack()
{
	NodeName = "Attack";
}
//Eseguire Task
EBTNodeResult::Type UMyBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//null Check
	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	//Ottengo player
	AStealth_PrototypeEnemy* Character = Cast<AStealth_PrototypeEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	//null check player
	if (Character == nullptr)
		return EBTNodeResult::Failed;
	//ritorno succeeded
	return EBTNodeResult::Succeeded;
}
