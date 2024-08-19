// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PPAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPPAT_Trace();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UPPAT_Trace* CreateTask(UGameplayAbility* OwnerAbility);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	void TargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);
};
