// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StealthAIController.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API AStealthAIController : public AAIController
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
	
protected:
	virtual void BeginPlay() override;

public:
	bool IsDead() const;

};
