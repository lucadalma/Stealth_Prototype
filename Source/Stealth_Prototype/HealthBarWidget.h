// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTH_PROTOTYPE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Costruttore
	virtual void NativeConstruct() override;
	//Funzione per settare la healthbar
	void SetBarValuePercent(float const value);
private:
	//Batta della vita
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess="true"))
	UProgressBar* HealthValue = nullptr;
};
