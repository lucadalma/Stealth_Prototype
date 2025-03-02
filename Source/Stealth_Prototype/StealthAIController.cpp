// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Stealth_PrototypeCharacter.h"
#include "Enemy.h"

//Costruttore
AStealthAIController::AStealthAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}
//OnPossess
void AStealthAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//Ottengo Eneny
	if (AEnemy* const Enemy = Cast<AEnemy>(InPawn))
	{
		//Ottengo BehaviorTree e llo faccio runnare
		if (UBehaviorTree* const Tree = Enemy->GetBehaviorTree())
		{
			UBlackboardComponent* BC;
			UseBlackboard(Tree->BlackboardAsset, BC);
			Blackboard = BC;
			RunBehaviorTree(Tree);
		}
	}

}

//Funzione per impostare il sistema di percezzione dell'AI 
void AStealthAIController::SetupPerceptionSystem()
{
	//Creo il sistema di percezione visiva
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	//Check
	if (SightConfig)
	{
		//Creo il sistema di percezione
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		//Il raggio della vista
		SightConfig->SightRadius = 500.f;
		//Raggio di transizione tra visto e non visto
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		//Angolo del campo visivo dell'AI
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		//Quando permane l'informazione di aver visto
		SightConfig->SetMaxAge(5.f);
		//Se il giocatore è a 520 unità dall'ultima posizione vista l'IA lo rileva automaticamente
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		//Configura l'IA per rilevare tutti i tipi di attori
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		//Aggiungo il senso della vista alla componente di percezione
		GetPerceptionComponent()->ConfigureSense((*SightConfig));
	}
	//Questo per il sistema di udito
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	//Check
	if (HearingConfig)
	{
		//Raggio di ascolto
		HearingConfig->HearingRange = 600.f;
		//Se il suono è in linea di vista, il range aumenta a 800 unità
		HearingConfig->LoSHearingRange = 800.f;
		//Usa la linea di vista per determinare la percezione
		HearingConfig->bUseLoSHearing = true;
		//Qunato permane l'aver sentito
		HearingConfig->SetMaxAge(5.f);
		//Configura l'IA per rilevare tutti i tipi di attori
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		//Aggiungo il senso dell'udito alla componente di percezione
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
	//Imposto un senso dominante
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//Ogni volta che l'IA rileva il player viene chiamata OnTargetDetected
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AStealthAIController::OnTargetDetected);
}
//Funzione per qunado rivela il player
void AStealthAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	//Ottengo il player
	if (auto* const Player = Cast<AStealth_PrototypeCharacter>(Actor))
	{
		//Se lo vede
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			//Imposto le variabili nella BlackBoard
			GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
			GetBlackboardComponent()->SetValueAsVector("LastKnownPlayerPosition", Player->GetActorLocation());
		}
		else 
		{
			//Se l'IA non lo vede più avvia la fase di investigazione
			GetBlackboardComponent()->SetValueAsBool("IsInvestigating", true);
		}

		//Se lo sente
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			//Imposto le variabili nella BlackBoard
			GetBlackboardComponent()->SetValueAsVector("HeardLocation", Stimulus.StimulusLocation);
			GetBlackboardComponent()->SetValueAsBool("HeardNoise", Stimulus.WasSuccessfullySensed());
		}

	}
}