// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"

APPGASPlayerState::APPGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UPPCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* APPGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
