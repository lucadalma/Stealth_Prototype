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
	//Ottengo Ai Controller
	if (auto* const AI = Cast<AStealthAIController>(OwnerComp.GetAIOwner()))
	{
		//Ottengo il BlackBoard
		if (auto* const BC = OwnerComp.GetBlackboardComponent())
		{
			//Ottengo l'attuale index del patrol point
			auto const index = BC->GetValueAsInt((GetSelectedBlackboardKey()));

			//Ottengo l'Enemy
			if (auto* Enemy = Cast<AEnemy>(AI->GetPawn()))
			{
				//Ottieni il vettore attuale del percorso di pattuglia dall' Enemy
				auto const Point = Enemy->GetPatrolPath()->GetPatrolPoint(index);

				auto const GlobalPoint = Enemy->GetPatrolPath()->GetActorTransform().TransformPosition(Point);
				//Setto il path point
				BC->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				//Finisco con un successo
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
