// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API UMyBTService_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	

public:
	//Costruttore
	UMyBTService_ChangeSpeed();
	//Funzione Service
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//Varibile speed (AllowPrivateAccess = "yes"):Permette l'accesso alla variabile anche se è dichiarata come private nella classe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "yes"))
	float Speed = 600.f;

};
