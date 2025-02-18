// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPGruntAttributeSet.h"
#include "AI/PPAIController.h"

APPGASCharacterNonPlayer::APPGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UPPGruntAttributeSet>(TEXT("AttributeSet"));

//AI
	static ConstructorHelpers::FClassFinder<AController> AIControllerRef(TEXT("/Script/CoreUObject.Class'/Script/Project_P.PPAIController'"));
	if (AIControllerRef.Class)
	{
		AIControllerClass = AIControllerRef.Class;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

	for (const TSubclassOf<UGameplayAbility>& StartAbility : StartAbilites)
	{
		//ASC는 직접적으로 GA를 접근, 관리하는게 아닌
		//FGameplayAbilitySpec 구조체를 통해 간접적으로 관리함
		FGameplayAbilitySpec Spec(StartAbility);

		ASC->GiveAbility(Spec);
	}

	for (const TPair<EInputAbility, TSubclassOf<class UGameplayAbility>>& StartInputAbility : StartInputAbilites)
	{
		FGameplayAbilitySpec Spec(StartInputAbility.Value);

		Spec.InputID = (int32)StartInputAbility.Key;

		ASC->GiveAbility(Spec);
	}

	ASC->GetGameplayAttributeValueChangeDelegate(UPPGruntAttributeSet::GetDamageAttribute()).
		AddUObject(this, &APPGASCharacterNonPlayer::TakeDamage);
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

void APPGASCharacterNonPlayer::TakeDamage(const FOnAttributeChangeData& ChangeData)
{
}
