// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stealth_PrototypeCharacter.h"


void AStealthAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior == nullptr) return;

	RunBehaviorTree(AIBehavior);

}




bool AStealthAIController::IsDead() const
{
	AStealth_PrototypeCharacter* ControlledCharacter = Cast<AStealth_PrototypeCharacter>(GetPawn());

	if (ControlledCharacter != nullptr)
		return ControlledCharacter->IsDead();

	return true;
}