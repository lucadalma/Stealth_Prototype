// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class STEALTH_PROTOTYPE_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	//Costruttore
	APatrolPath();
	//Funzione Get point
	FVector GetPatrolPoint(int const index) const;
	//Funzione numero point
	int Num() const;

private:
	//Array di Vector Dei patrol points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(MakeEditWidget="true", AllowPrivateAccess="true"))
	TArray<FVector> PatrolPoints;

};
