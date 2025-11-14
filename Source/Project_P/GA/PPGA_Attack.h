// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "PPGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPPGA_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION()
	void OnCompletedCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	UFUNCTION()
	void OnInputOpen(FGameplayEventData Payload);

	UFUNCTION(Server, Reliable)
	void ServerRPC_InputReceived();

	void AdvanceComboAttack(UAbilitySystemComponent* ASC);

//ÄÞº¸ ¼½¼Ç
protected:
	UPROPERTY(VisibleAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<class UPPComboActionData> ComboActionData;

	TWeakObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask;
	TWeakObjectPtr<class UAbilityTask_WaitGameplayEvent> WaitInputOpenTask;

	FGameplayTag EventInputOpenTag = PPTAG_CHARACTER_EVENT_INPUTOPEN;
	FGameplayTag EventInputReceiveTag = PPTAG_CHARACTER_EVENT_INPUTRECEVIE;

	uint8 CurrentCombo = 0;

protected:
	void HandleCombo();
	void HandleInputReceive();
	FName GetNextSection();
};
