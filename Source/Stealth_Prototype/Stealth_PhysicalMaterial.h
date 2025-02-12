// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Stealth_PhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API UStealth_PhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PhysicalMaterial")
	class USoundBase* FootStepSound = nullptr;

};
