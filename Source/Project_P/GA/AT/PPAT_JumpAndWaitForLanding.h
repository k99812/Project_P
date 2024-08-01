// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PPAT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWatingForLandingDelegate);

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPPAT_JumpAndWaitForLanding();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", 
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UPPAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwingAbility);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FJumpAndWatingForLandingDelegate OnComplete;

protected:
	UFUNCTION()
	void OnLandedCallBack(const FHitResult& Hit);
};
