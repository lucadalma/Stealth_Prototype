// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stealth_PrototypeCharacter.h"
#include "Enemy.h"


AStealthAIController::AStealthAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AStealthAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AEnemy* const npc = Cast<AEnemy>(InPawn))
	{

		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{

			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}

}

void AStealthAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{

		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 600.f; // Adjust range as needed
		HearingConfig->LoSHearingRange = 800.f; // Line-of-sight range for better detection
		HearingConfig->bUseLoSHearing = true;
		HearingConfig->SetMaxAge(5.f);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AStealthAIController::OnTargetDetected);
}

void AStealthAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const Player = Cast<AStealth_PrototypeCharacter>(Actor))
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
			GetBlackboardComponent()->SetValueAsVector("LastKnownPlayerPosition", Player->GetActorLocation());
		}
		else 
		{
			GetBlackboardComponent()->SetValueAsBool("IsInvestigating", true);
		}

		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			GetBlackboardComponent()->SetValueAsVector("HeardLocation", Stimulus.StimulusLocation);
			GetBlackboardComponent()->SetValueAsBool("HeardNoise", Stimulus.WasSuccessfullySensed());
		}
	}
}