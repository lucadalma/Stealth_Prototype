// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

//Costuttore
void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
//Setta valore barra
void UHealthBarWidget::SetBarValuePercent(float const value) 
{
	HealthValue->SetPercent(value);
}