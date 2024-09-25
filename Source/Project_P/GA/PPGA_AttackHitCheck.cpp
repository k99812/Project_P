// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/PPAT_Trace.h"
#include "GA/TA/PPTA_Trace.h"
#include "Project_P.h"
#include "Attribute/PPCharacterAttributeSet.h"

UPPGA_AttackHitCheck::UPPGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPPGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//#include "GA/AT/PPAT_Trace.h" 추가
	//#include "GA/TA/PPTA_Trace.h" 추가
	UPPAT_Trace* AttackTraceTask = UPPAT_Trace::CreateTask(this, APPTA_Trace::StaticClass());

	AttackTraceTask->OnComplete.AddDynamic(this, &UPPGA_AttackHitCheck::TraceResultCallback);

	AttackTraceTask->ReadyForActivation();
}

void UPPGA_AttackHitCheck::TraceResultCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	//#include "AbilitySystemBlueprintLibrary.h" 추가
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
		UPPCharacterAttributeSet* TargetAttributeSet = const_cast<UPPCharacterAttributeSet*>(TargetASC->GetSet<UPPCharacterAttributeSet>());
		if (!OwnerAttributeSet || !TargetAttributeSet)
		{
			PPGAS_LOG(LogGAS, Error, TEXT("(Owner or Target)AttributeSet Not Found"));
			return;
		}

		const float AttackDamage = OwnerAttributeSet->GetAttackRate();
		TargetAttributeSet->SetHealth(TargetAttributeSet->GetHealth() - AttackDamage);
	}

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
