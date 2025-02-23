// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CustomWait.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_CustomWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        FTimerHandle TimerHandle;
        OwnerComp.GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
            {
                BlackboardComp->SetValueAsBool("IsInvestigating", false);
            }, 3.0f, false);

        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}
