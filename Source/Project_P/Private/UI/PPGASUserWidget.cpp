// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

UAbilitySystemComponent* UPPGASUserWidget::GetAbilitySystemComponent() const
{
    return ASC;
}

void UPPGASUserWidget::SetAbilitySystemComponent(AActor* Owner)
{
    if (IsValid(Owner))
    {
        ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
    }
}
