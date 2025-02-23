// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_CustomWait.h"
#include "StealthAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

UMyBTTask_CustomWait::UMyBTTask_CustomWait()
{
    NodeName = "Custom Wait Task";
}

EBTNodeResult::Type UMyBTTask_CustomWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AStealthAIController* AIController = Cast<AStealthAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTTask_CustomWait: AIController is nullptr!"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTTask_CustomWait: BlackboardComponent is nullptr!"));
        return EBTNodeResult::Failed;
    }

    // Imposta un Timer per aspettare 3 secondi e poi completare il task
    FTimerHandle TimerHandle;
    AIController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AIController, BlackboardComp, &OwnerComp]()
        {
            if (BlackboardComp) // Controlla che esista ancora
            {
                BlackboardComp->SetValueAsBool("IsInvestigating", false);
            }

            // Concludi il task
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }, 3.0f, false);

    return EBTNodeResult::InProgress;
}

