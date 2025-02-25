// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_IsPlayerInMeleeRange.h"
#include "StealthAIController.h"
#include "Enemy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"


UMyBTService_IsPlayerInMeleeRange::UMyBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UMyBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const * const cont = Cast<AStealthAIController>(OwnerComp.GetAIOwner());
	auto const * const enemy = Cast<AEnemy>(cont->GetPawn());

	auto const * const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), enemy->GetDistanceTo(player) <= MeleeRange);


}
