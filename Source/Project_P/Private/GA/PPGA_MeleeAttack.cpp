// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_MeleeAttack.h"
#include "Interface/PPGASInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Data/PPComboActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "Perception/AISense_Damage.h"
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

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		if (HasAuthority(&ActivationInfo) && !IsLocallyControlled())
		{
			ASC->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).
				AddUObject(this, &UPPGA_MeleeAttack::OnTargetDataReceived);
			ASC->AbilityTargetDataCancelledDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).
				AddUObject(this, &UPPGA_MeleeAttack::OnTargetDataCancelled);
		}
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

	if (WaitHitEventTask.IsValid())
	{
		WaitHitEventTask.Get()->EndTask();
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(EventInputOpenTag);
		ASC->RemoveLooseGameplayTag(EventInputReceiveTag);
		ASC->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).RemoveAll(this);
		ASC->AbilityTargetDataCancelledDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).RemoveAll(this);
	}

	ComboAttackMontage = nullptr;
	ComboActionData = nullptr;

	WaitHitEventTask = nullptr;
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
		//PPNET_SUBLOG(LogGAS, Log, TEXT("Listhen Begin"));

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

	if (WaitHitEventTask.IsValid())
	{
		WaitHitEventTask.Get()->EndTask();
	}

	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

	HandleCombo();
}

void UPPGA_MeleeAttack::HandleCombo()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

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

	if (IsLocallyControlled())
	{
		UAbilityTask_WaitGameplayEvent* WaitHitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PPTAG_EVENT_MELEEHIT);
		WaitHitEvent->EventReceived.AddDynamic(this, &UPPGA_MeleeAttack::OnHitReceived);
		WaitHitEvent->ReadyForActivation();
		WaitHitEventTask = WaitHitEvent;
	}

	PlayMontageTask->ReadyForActivation();
}

void UPPGA_MeleeAttack::HandleInputReceive()
{
	if (!CurrentActorInfo)
	{
		return;
	}

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
	//애님 노티파이 히트 이벤트 수신
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

	if (Payload.TargetData.Num() == 0)
	{
		return;
	}

	if (HasAuthority(&CurrentActivationInfo))
	{
		//몬스터 or 리슨 서버 클라
		OnTargetDataReceived(Payload.TargetData, FGameplayTag());
	}
	else if (IsLocallyControlled())
	{
		//로컬 클라이언트
		FScopedPredictionWindow ScopedPredictionWindow(ASC, true);

		ASC->ServerSetReplicatedTargetData(
			CurrentSpecHandle, 
			CurrentActivationInfo.GetActivationPredictionKey(),
			Payload.TargetData, 
			PPTAG_EVENT_MELEEHIT, 
			ASC->ScopedPredictionKey
		);
	}
}

void UPPGA_MeleeAttack::OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

	PPNET_SUBLOG(LogGAS, Log, TEXT("Target Data Received"));

	ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(DataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(DataHandle, 0);

		PPGAS_LOG(LogGAS, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		if (!OwnerASC || !TargetASC)
		{
			PPGAS_LOG(LogGAS, Error, TEXT("(Owner or Target)ASC Not Found"));
			return;
		}

		const UPPCharacterAttributeSet* OwnerAttributeSet = OwnerASC->GetSet<UPPCharacterAttributeSet>();
		const UPPCharacterAttributeSet* TargetAttributeSet = TargetASC->GetSet<UPPCharacterAttributeSet>();
		if (!OwnerAttributeSet || !TargetAttributeSet)
		{
			PPGAS_LOG(LogGAS, Error, TEXT("(Owner or Target)AttributeSet Not Found"));
			return;
		}

		AActor* Owner = GetAvatarActorFromActorInfo();
		AActor* Target = HitResult.GetActor();
		if (!Owner || !Target)
		{
			PPGAS_LOG(LogGAS, Error, TEXT("(Owner or Target)Actor Not Found"));
			return;
		}

		float AttackRange = OwnerAttributeSet->GetAttackRange();
		float Tolerance = 200.0f;
		float Distance = Owner->GetSquaredDistanceTo(Target);
		if (Distance > (AttackRange + Tolerance) * (AttackRange + Tolerance))
		{
			PPGAS_LOG(LogGAS, Warning, TEXT("Validation Failed: Too Far (Dist: %.2f, Max: %.2f)"), FMath::Sqrt(Distance), AttackRange + Tolerance);
			return;
		}

		if (TargetASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD))
		{
			PPGAS_LOG(LogGAS, Log, TEXT("Validation Failed: Target Already Dead"));
			return;
		}

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, (float)CurrentCombo);
		if (SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, DataHandle);
		}

		//타겟이 몬스터일 경우 AI 데미지 센스 발동
		IGameplayTagAssetInterface* Monster = Cast<IGameplayTagAssetInterface>(HitResult.GetActor());
		if (Monster && Monster->HasMatchingGameplayTag(PPTAG_CHARACTER_MONSTER))
		{
			UAISense_Damage::ReportDamageEvent(this, HitResult.GetActor(), OwnerASC->GetAvatarActor(),
				OwnerAttributeSet->GetAttackRate(), HitResult.GetActor()->GetActorLocation(), HitResult.Location);
		}
	}
}

void UPPGA_MeleeAttack::OnTargetDataCancelled()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return;
	}

	PPNET_SUBLOG(LogGAS, Log, TEXT("Target Data Cancelled"));
}
