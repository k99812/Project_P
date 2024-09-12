// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"

APPGASCharacterNonPlayer::APPGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* APPGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void APPGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
}
