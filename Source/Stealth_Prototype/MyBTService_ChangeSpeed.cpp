// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_ChangeSpeed.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

UMyBTService_ChangeSpeed::UMyBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UMyBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (auto const Cont = OwnerComp.GetAIOwner())
	{

		if (auto* const NPC = Cast<AEnemy>(Cont->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
