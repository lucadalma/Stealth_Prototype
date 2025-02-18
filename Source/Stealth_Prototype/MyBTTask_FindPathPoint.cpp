// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FindPathPoint.h"
#include "StealthAIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_FindPathPoint::UMyBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UMyBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// attempt to get the NPC's controller
	if (auto* const cont = Cast<AStealthAIController>(OwnerComp.GetAIOwner()))
	{
		// attempt to get the blackboard component from the behaviour tree
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			// get the current patrol path index from the blackboard
			auto const index = bc->GetValueAsInt((GetSelectedBlackboardKey()));

			// get the NPC
			if (auto* npc = Cast<AEnemy>(cont->GetPawn()))
			{
				// get the current patrol path vector from the NPC - this is local to the patrol path actor
				auto const Point = npc->GetPatrolPath()->GetPatrolPoint(index);

				auto const GlobalPoint = npc->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
