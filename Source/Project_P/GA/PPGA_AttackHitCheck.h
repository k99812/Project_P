// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PPGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPPGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void TraceResultCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

//GE Section
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	float CurrentLevel;
};
