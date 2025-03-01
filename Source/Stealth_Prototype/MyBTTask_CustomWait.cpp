// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_CustomWait.h"
#include "StealthAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

//Cosatruttore
UMyBTTask_CustomWait::UMyBTTask_CustomWait()
{
	NodeName = "Custom Wait Task";
}
//ExecuteTask
EBTNodeResult::Type UMyBTTask_CustomWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Ottenfo AI Controller
	AStealthAIController* AIController = Cast<AStealthAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	//Ottengo BlackboardComponent
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// Imposta un Timer per aspettare 3 secondi e poi completare il task
	FTimerHandle TimerHandle;
	AIController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AIController, BlackboardComp, &OwnerComp]()
		{
			//Controlla che esista ancora
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsBool("IsInvestigating", false);
			}

			// Concludi il task
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}, 3.0f, false);

	return EBTNodeResult::InProgress;
}

