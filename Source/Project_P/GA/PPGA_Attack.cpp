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

//Movement Mode 설정
	//PPCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if (IsValid(ComboAttackMontage))
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ComboAttackMontage, 1.0f, GetNextSection());
		PlayAttackTask->OnCompleted.AddDynamic(this, &UPPGA_Attack::OnCompletedCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UPPGA_Attack::OnInterruptedCallback);
		PlayAttackTask->ReadyForActivation();

		StartTimer();
	}
}

void UPPGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	UE_LOG(LogTemp, Log, TEXT("CancelAbility : Attack"));
}

void UPPGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	APPCharacterBase* PPCharacter = CastChecked<APPCharacterBase>(ActorInfo->AvatarActor.Get());

//Movement Mode 설정
	//PPCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	ComboActionData = nullptr;
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

FName UPPGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UPPGA_Attack::StartTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(CurrentCombo));

	const float ComboEffectiveTime = ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate;
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
		MontageJumpToSection(GetNextSection());
		StartTimer();
		HasNextAttackInput = false;
	}
}
