// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_ChangeSpeed.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

//Service Node Name
UMyBTService_ChangeSpeed::UMyBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}
//Cambio la velocità dell'AI in base a dove si trova nel BehaviorTree
void UMyBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (auto const AI = OwnerComp.GetAIOwner())
	{
		//Ottendo Enemy
		if (auto* const Enemy = Cast<AEnemy>(AI->GetPawn()))
		{
			Enemy->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
