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
	explicit AStealthAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus); 
};