// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_Attack.h"
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

UPPGA_Attack::UPPGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//클라이언트 예측
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	//리플리케이션 정책
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UPPGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

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

void UPPGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	//UE_LOG(LogTemp, Log, TEXT("CancelAbility : Attack"));
}

void UPPGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (MontageTask.IsValid())
	{
		MontageTask.Get()->EndTask();
	}

	if (WaitInputOpenTask.IsValid())
	{
		WaitInputOpenTask.Get()->EndTask();
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();

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

void UPPGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		PPNET_SUBLOG(LogGAS, Log, TEXT("Listhen Begin"));
		HandleInputReceive();
	}
	else
	{
		PPNET_SUBLOG(LogGAS, Log, TEXT("Client Begin"));
		ServerRPC_InputReceived();
		HandleInputReceive();
	}
}

void UPPGA_Attack::ServerRPC_InputReceived_Implementation()
{
	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	HandleInputReceive();
}

void UPPGA_Attack::HandleInputReceive()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();

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

void UPPGA_Attack::OnCompletedCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPPGA_Attack::OnInterruptedCallback()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPPGA_Attack::OnInputOpen(FGameplayEventData Payload)
{
	if (WaitInputOpenTask.IsValid())
	{
		WaitInputOpenTask.Get()->EndTask();
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();

	if (ASC)
	{
		ASC->AddLooseGameplayTag(EventInputOpenTag);

		if (ASC->HasMatchingGameplayTag(EventInputReceiveTag))
		{
			AdvanceComboAttack(ASC);
		}
	}
}

void UPPGA_Attack::HandleCombo()
{
	FName NextSection = GetNextSection();

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ComboAttackMontage, 1.0f, NextSection);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UPPGA_Attack::OnCompletedCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UPPGA_Attack::OnInterruptedCallback);
	MontageTask = PlayMontageTask;
	
	if (CurrentCombo < ComboActionData->MaxComboCount)
	{
		UAbilityTask_WaitGameplayEvent* WaitInputOpen = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PPTAG_EVENT_INPUTOPEN);
		WaitInputOpen->EventReceived.AddDynamic(this, &UPPGA_Attack::OnInputOpen);
		WaitInputOpen->ReadyForActivation();
		WaitInputOpenTask = WaitInputOpen;
	}

	PlayMontageTask->ReadyForActivation();

	if (HasAuthority(&CurrentActivationInfo)) 
	{
		PPCharacter->Multicast_SendPlayMontage(NextSection);
	}
}

void UPPGA_Attack::AdvanceComboAttack(UAbilitySystemComponent* ASC)
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

FName UPPGA_Attack::GetNextSection()
{
	FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}