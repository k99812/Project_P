// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_MeleeAttack.h"
#include "Interface/PPGASInterface.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Data/PPComboActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tag/PPGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Project_P.h"

UPPGA_MeleeAttack::UPPGA_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//클라이언트 예측
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	//리플리케이션 정책
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UPPGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	PPCharacter = ActorInfo->AvatarActor.Get();
	if (PPCharacter)
	{
		ComboAttackMontage = PPCharacter->GetComboAttackMontage();
		ComboActionData = PPCharacter->GetComboActionData();
	}

	if (IsValid(ComboAttackMontage))
	{
		CurrentCombo = 1;

		HandleCombo();
	}
}

void UPPGA_MeleeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	//UE_LOG(LogTemp, Log, TEXT("CancelAbility : Attack"));
}

void UPPGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (MontageTask.IsValid())
	{
		MontageTask.Get()->EndTask();
	}

	if (WaitInputOpenTask.IsValid())
	{
		WaitInputOpenTask.Get()->EndTask();
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(EventInputOpenTag);
		ASC->RemoveLooseGameplayTag(EventInputReceiveTag);
	}

	ComboAttackMontage = nullptr;
	ComboActionData = nullptr;
	WaitInputOpenTask = nullptr;
	MontageTask = nullptr;
	PPCharacter = nullptr;
	CurrentCombo = 0;
}

void UPPGA_MeleeAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (ActorInfo->IsNetAuthority())
	{
		PPNET_SUBLOG(LogGAS, Log, TEXT("Listhen Begin"));

		if (!ASC->HasMatchingGameplayTag(EventInputReceiveTag))
		{
			HandleInputReceive();
		}
	}
	else
	{
		PPNET_SUBLOG(LogGAS, Log, TEXT("Client Begin"));

		if (!ASC->HasMatchingGameplayTag(EventInputReceiveTag))
		{
			ServerRPC_InputReceived();
			HandleInputReceive();
		}
	}
}

void UPPGA_MeleeAttack::ServerRPC_InputReceived_Implementation()
{
	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	HandleInputReceive();
}

void UPPGA_MeleeAttack::OnCompletedCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPPGA_MeleeAttack::OnInterruptedCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPPGA_MeleeAttack::OnInputOpen(FGameplayEventData Payload)
{
	if (!CurrentActorInfo) return;

	if (WaitInputOpenTask.IsValid())
	{
		WaitInputOpenTask.Get()->EndTask();
	}

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();

	if (ASC)
	{
		ASC->AddLooseGameplayTag(EventInputOpenTag);

		if (ASC->HasMatchingGameplayTag(EventInputReceiveTag))
		{
			AdvanceComboAttack(ASC);
		}
	}
}

void UPPGA_MeleeAttack::AdvanceComboAttack(UAbilitySystemComponent* ASC)
{
	ASC->RemoveLooseGameplayTag(EventInputOpenTag);
	ASC->RemoveLooseGameplayTag(EventInputReceiveTag);

	if (MontageTask.IsValid())
	{
		MontageTask.Get()->EndTask();
	}

	if (WaitInputOpenTask.IsValid())
	{
		WaitInputOpenTask.Get()->EndTask();
	}

	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

	HandleCombo();
}

void UPPGA_MeleeAttack::HandleCombo()
{
	FName NextSection = GetNextSection();

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ComboAttackMontage, 1.0f, NextSection);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UPPGA_MeleeAttack::OnCompletedCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UPPGA_MeleeAttack::OnInterruptedCallback);
	MontageTask = PlayMontageTask;

	if (CurrentCombo < ComboActionData->MaxComboCount)
	{
		UAbilityTask_WaitGameplayEvent* WaitInputOpen = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PPTAG_EVENT_INPUTOPEN);
		WaitInputOpen->EventReceived.AddDynamic(this, &UPPGA_MeleeAttack::OnInputOpen);
		WaitInputOpen->ReadyForActivation();
		WaitInputOpenTask = WaitInputOpen;
	}

	PlayMontageTask->ReadyForActivation();
}

void UPPGA_MeleeAttack::HandleInputReceive()
{
	if (!CurrentActorInfo) return;

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();

	if (ASC)
	{
		if (!ASC->HasMatchingGameplayTag(EventInputReceiveTag))
		{
			ASC->AddLooseGameplayTag(EventInputReceiveTag);
		}

		if (ASC->HasMatchingGameplayTag(EventInputOpenTag))
		{
			AdvanceComboAttack(ASC);
		}
	}
}

FName UPPGA_MeleeAttack::GetNextSection()
{
	FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UPPGA_MeleeAttack::OnHitReceived(FGameplayEventData Payload)
{
	if (Payload.TargetData.Num() == 0) return;

	if (HasAuthority(&CurrentActivationInfo))
	{

	}
	else
	{

	}

	if (IsLocallyControlled())
	{

	}
}