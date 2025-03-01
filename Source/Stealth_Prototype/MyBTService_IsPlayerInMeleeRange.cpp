// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_IsPlayerInMeleeRange.h"
#include "StealthAIController.h"
#include "Enemy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

//Costruttore
UMyBTService_IsPlayerInMeleeRange::UMyBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UMyBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Ottendo Controller AI
	auto const * const AI = Cast<AStealthAIController>(OwnerComp.GetAIOwner());
	//Ottengo dal controller l'enemy
	auto const * const Enemy = Cast<AEnemy>(AI->GetPawn());
	//Ottengo il player
	auto const * const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//Ritorno un bool se in range alla variabile GetSelectedBlackboardKey()
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Enemy->GetDistanceTo(Player) <= MeleeRange);


}
