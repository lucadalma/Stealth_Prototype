#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "StealthAIController.generated.h"

UCLASS()
class STEALTH_PROTOTYPE_API AStealthAIController : public AAIController
{
	GENERATED_BODY()
public:
	//Costruttore
	explicit AStealthAIController(FObjectInitializer const& ObjectInitializer);

protected:
	//Funzione OnPossess
	virtual void OnPossess(APawn* InPawn) override;
private:
	//Vista
	class UAISenseConfig_Sight* SightConfig;
	//Udito
	class UAISenseConfig_Hearing* HearingConfig;
	//Funzione per settare il sistema di percezione
	void SetupPerceptionSystem();
	//Funzione per quando trova il player
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus); 
};