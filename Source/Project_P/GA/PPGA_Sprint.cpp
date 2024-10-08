// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_Sprint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tag/PPGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Character/PPGASCharacterPlayer.h"

UPPGA_Sprint::UPPGA_Sprint() : SprintSpeed(1000.0f), WalkSpeed(500.0f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPPGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = SprintSpeed;
	}
}

void UPPGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = WalkSpeed;
	}
}

void UPPGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UPPGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bCanActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bCanActivate)
	{
		return false;
	}
	
	APPGASCharacterPlayer* Owner = CastChecked<APPGASCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();

	if (ASC)
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

		bCanActivate = ASC->HasAnyMatchingGameplayTags(TagContainer);
	}
	
	return bCanActivate;
}


