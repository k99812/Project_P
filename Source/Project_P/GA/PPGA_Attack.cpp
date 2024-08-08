// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_Attack.h"
#include "Character/PPCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Data/PPComboActionData.h"
#include "GameFramework/CharacterMovementComponent.h"

UPPGA_Attack::UPPGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPPGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	APPCharacterBase* PPCharacter = CastChecked<APPCharacterBase>(ActorInfo->AvatarActor.Get());
	ComboAttackMontage = PPCharacter->GetComboAttackMontage();
	ComboActionData = PPCharacter->GetComboActionData();
	PPCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if (IsValid(ComboAttackMontage[CurrentCombo]))
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ComboAttackMontage[CurrentCombo]);
		PlayAttackTask->OnCompleted.AddDynamic(this, &UPPGA_Attack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UPPGA_Attack::OnInterruptedCallback);
		PlayAttackTask->ReadyForActivation();
	}

	StartTimer();
}

void UPPGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);


}

void UPPGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	APPCharacterBase* PPCharacter = CastChecked<APPCharacterBase>(ActorInfo->AvatarActor.Get());
	PPCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	CurrentCombo = 0;
	HasNextAttackInput = false;
}

void UPPGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UE_LOG(LogTemp, Log, TEXT("attack input pressed"));
	
	if (!ComboTimerHandle.IsValid())
	{
		HasNextAttackInput = false;
	}
	else
	{
		HasNextAttackInput = true;
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

void UPPGA_Attack::StartTimer()
{
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(CurrentCombo));

	const float ComboEffectiveTime = ComboActionData->EffectiveFrameCount[CurrentCombo] / ComboActionData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UPPGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UPPGA_Attack::CheckComboInput()
{
	//타이머 종료
	ComboTimerHandle.Invalidate();
	if (HasNextAttackInput)
	{
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 0, ComboActionData->MaxComboCount);
		if (IsValid(ComboAttackMontage[CurrentCombo]))
		{
			UE_LOG(LogTemp, Log, TEXT("IsValid"));
			UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ComboAttackMontage[CurrentCombo]);
			PlayAttackTask->OnCompleted.AddDynamic(this, &UPPGA_Attack::OnCompletedCallback);
			PlayAttackTask->OnInterrupted.AddDynamic(this, &UPPGA_Attack::OnInterruptedCallback);
			PlayAttackTask->ReadyForActivation();
		}
		StartTimer();
		HasNextAttackInput = false;
	}
}
