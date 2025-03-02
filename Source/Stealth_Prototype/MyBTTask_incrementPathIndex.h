// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_incrementPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API UMyBTTask_incrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	//Costruttore
	explicit UMyBTTask_incrementPathIndex(FObjectInitializer const& ObjectInitializer);
	//ExecuteTask
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	//Enum per la direzione
	enum class EDirectionType { Forward, Reverse };
	//Variabile direzione
	EDirectionType Direction = EDirectionType::Forward;

	//Variabile per capire se cambiare direzione quando si trova al primo o all'ultimo index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool bBiDirectional = false;

};
