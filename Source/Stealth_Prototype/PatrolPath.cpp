// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

//Costruttore
APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;
}

//Ritorno un preciso patrol point
FVector APatrolPath::GetPatrolPoint(int const index) const
{
	return PatrolPoints[index];
}
//Ritorno il numero di patrol point
int APatrolPath::Num() const
{
	return PatrolPoints.Num();
}

