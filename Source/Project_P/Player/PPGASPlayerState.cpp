// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPGASPlayerState.h"
#include "AbilitySystemComponent.h"

APPGASPlayerState::APPGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* APPGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
