// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_GASPlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UAbilityTask_GASPlayMontage : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()
	
public:
	UAbilityTask_GASPlayMontage();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMontageAndWait",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_GASPlayMontage* CreatePlayMontage(UGameplayAbility* Abilitys,
		FName TaskName, UAnimMontage* Montage, float Rates = 1.f, FName start = NAME_None, bool bStopAbilityEnds = true, float AnimTranslationScale = 1.f, float StartSeconds = 0.f, bool bAllowInterrupt = false);

	virtual void Activate() override;
};
