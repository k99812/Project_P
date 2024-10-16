// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"

APPGASCharacterNonPlayer::APPGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UPPCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* APPGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void APPGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->ActorIsDead.AddDynamic(this, &APPGASCharacterNonPlayer::ActorIsDead);
}

void APPGASCharacterNonPlayer::ActorIsDead()
{
	SetDead();
}

void APPGASCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
	{
		Destroy();
	}
	), DeadEventDelayTime, false);
}
