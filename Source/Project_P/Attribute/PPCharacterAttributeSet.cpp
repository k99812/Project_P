// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PPCharacterAttributeSet.h"

UPPCharacterAttributeSet::UPPCharacterAttributeSet() : 
	AttackRange(100.0f), MaxAttackRange(300.0f),
	AttackRadius(50.0f), MaxAttackRadius(150.0f),
	AttackRate(30.0f), MaxAttackRate(100.0f),
	MaxHealth(100.0f)
{
	InitHealth(GetMaxHealth());
}

bool UPPCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UPPCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
}

void UPPCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
}

void UPPCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
}
