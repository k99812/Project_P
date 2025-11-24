// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/PPGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/PPAT_Trace.h"
#include "GA/TA/PPTA_Trace.h"
#include "Project_P.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Tag/PPGameplayTag.h"
#include "GameplayTagAssetInterface.h"
#include "Perception/AISense_Damage.h"

UPPGA_AttackHitCheck::UPPGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UPPGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));
	CurrentLevel = TriggerEventData->EventMagnitude;

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

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (SpecHandle.IsValid() && HasAuthority(&CurrentActivationInfo))
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, DataHandle);
		}

		//Hit 이벤트 발생
		FGameplayEventData PayLoadData;
		PayLoadData.Instigator = OwnerASC->GetAvatarActor();
		PayLoadData.Target = TargetASC->GetAvatarActor();
		OwnerASC->HandleGameplayEvent(PPTAG_ABILITY_HIT, &PayLoadData);
		//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerASC->GetAvatarActor(), PPTAG_ABILITY_HIT, PayLoadData);

		//타겟이 몬스터일 경우 AI 데미지 센스 발동
		IGameplayTagAssetInterface* TargetActor = Cast<IGameplayTagAssetInterface>(HitResult.GetActor());
		if (TargetActor && TargetActor->HasMatchingGameplayTag(PPTAG_CHARACTER_MONSTER))
		{
			UAISense_Damage::ReportDamageEvent(this, HitResult.GetActor(), OwnerASC->GetAvatarActor(),
				OwnerAttributeSet->GetAttackRate(), HitResult.GetActor()->GetActorLocation(), HitResult.Location);
		}
	}

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
