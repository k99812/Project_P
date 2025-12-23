// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_Sprint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Tag/PPGameplayTag.h"
#include "Project_P.h"

UPPGA_Sprint::UPPGA_Sprint() : SprintSpeed(1000.0f), WalkSpeed(500.0f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//클라이언트 예측
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	//리플리케이션 정책
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UPPGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));
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
	//PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = WalkSpeed;
	}
}

void UPPGA_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	//PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));
	bool bReplicateEndAbility = true;

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility);
}

void UPPGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

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
	
	if (ActorInfo->IsLocallyControlled())
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC && !ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISWALKING))
		{
			return false;
		}
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	UCharacterMovementComponent* Movement = Character ? Character->GetCharacterMovement() : nullptr;
	
	if (Movement)
	{
		if (Movement->IsWalking() && Movement->Velocity.SizeSquared() > 10.0f &&
			Movement->GetCurrentAcceleration().SizeSquared() > 0.0f)
		{
			return true;
		}
	}

	return false;
}