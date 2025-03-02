// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyBTTask_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API UMyBTTask_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	//Costruttore
	UMyBTTask_MeleeAttack();
	//Execute Task
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//Funzione bool
	bool MontegeHasFinished(AEnemy* const enemy);
	
};
