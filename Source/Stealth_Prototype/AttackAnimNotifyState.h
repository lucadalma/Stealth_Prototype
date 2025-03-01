#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackAnimNotifyState.generated.h"

UCLASS()
class STEALTH_PROTOTYPE_API UAttackAnimNotifyState : public UAnimNotifyState
{
	
private:
	GENERATED_BODY()

public:
	//NotifyBegin
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	//NotifyEnd
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
