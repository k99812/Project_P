// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "PPTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	APPTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bSHowDebug = true;
};
