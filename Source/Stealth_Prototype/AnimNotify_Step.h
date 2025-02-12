// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Stealth_PrototypeCharacter.h"
#include "AnimNotify_Step.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API UAnimNotify_Step : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	EFoot Foot;
};
