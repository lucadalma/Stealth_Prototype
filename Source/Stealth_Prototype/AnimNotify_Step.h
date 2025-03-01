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
	//Intensità suono del passo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Loudness = 1.0f;

	//Range del passo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxRange = 1000.0f;
	//Funzione Notify
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	//Enum per piede sinisto o destro
	UPROPERTY(EditAnywhere)
	EFoot Foot;
};
