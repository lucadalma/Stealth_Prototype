// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FindRandomLocation.h"
#include "StealthAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UMyBTTask_FindRandomLocation::UMyBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UMyBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AStealthAIController>(OwnerComp.GetAIOwner()))
	{

		if (auto* const npc = cont->GetPawn())
		{
			// obtain npc location to use as an origin
			auto const Origin = npc->GetActorLocation();

			// get the navigatoin system and generate a random location
			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}
				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
