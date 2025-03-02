// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_incrementPathIndex.h"
#include "StealthAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"

//Costruttore
UMyBTTask_incrementPathIndex::UMyBTTask_incrementPathIndex(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Increment Path Index");
}

//ExexuteTask
EBTNodeResult::Type UMyBTTask_incrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Prendo AIController
	if (auto* const AI = Cast<AStealthAIController>(OwnerComp.GetAIOwner()))
	{
		//Prendo Enemy
		if (auto* const Enemy = Cast<AEnemy>(AI->GetPawn()))
		{
			//Prendo Blackboard
			if (auto* const BC = OwnerComp.GetBlackboardComponent())
			{
				//Prendo il numero di patrol point, e l'indice massimo e minimo1
				auto const NoOfPoints = Enemy->GetPatrolPath()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto Index = BC->GetValueAsInt(GetSelectedBlackboardKey());
				//Cambia direzione se siamo all'ultimo o al primo index
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}
				//Scrivo il nuovo index alla BlackBoard
				BC->SetValueAsInt(GetSelectedBlackboardKey(), (Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);

				//Finisco task
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	//Errore
	return EBTNodeResult::Failed;
}