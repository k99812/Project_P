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
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"

UPPGA_AttackHitCheck::UPPGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UPPGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));
	CurrentLevel = TriggerEventData->EventMagnitude;
	bool bIsMonster = false;

	IGameplayTagAssetInterface* Owner = Cast<IGameplayTagAssetInterface>(ActorInfo->AvatarActor.Get());
	if (Owner && Owner->HasMatchingGameplayTag(PPTAG_CHARACTER_MONSTER))
	{
		bIsMonster = true;
	}

	AGameplayAbilityTargetActor* SpawnedActor = nullptr;
	UAbilityTask_WaitTargetData* WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetData(this, FName("WaitTargetData"), EGameplayTargetingConfirmation::Instant, APPTA_Trace::StaticClass());
	
	WaitTargetData->ValidData.AddDynamic(this, &UPPGA_AttackHitCheck::TraceResultCallback);
	TargetActor = SpawnedActor;

	if (WaitTargetData->BeginSpawningActor(this, APPTA_Trace::StaticClass(), SpawnedActor))
	{
		APPTA_Trace* MyTrace = Cast<APPTA_Trace>(SpawnedActor);
		if (MyTrace)
		{
			MyTrace->SetShowDebug(true);
			MyTrace->ShouldProduceTargetDataOnServer = bIsMonster;
		}

		WaitTargetData->FinishSpawningActor(this, SpawnedActor);
	}

	WaitTargetData->ReadyForActivation();
}

void UPPGA_AttackHitCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (TargetActor.IsValid())
	{
		TargetActor = nullptr;
	}
}

void UPPGA_AttackHitCheck::TraceResultCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	if (HasAuthority(&CurrentActivationInfo))
	{
		ServerApplyHitLogic(DataHandle);
	}
	
	if(IsLocallyControlled())
	{
		ClientPlayHitCue(DataHandle);
	}

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPPGA_AttackHitCheck::ServerApplyHitLogic(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

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
		if (SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, DataHandle);
		}

		//타겟이 몬스터일 경우 AI 데미지 센스 발동
		IGameplayTagAssetInterface* Monster = Cast<IGameplayTagAssetInterface>(HitResult.GetActor());
		if (Monster && Monster->HasMatchingGameplayTag(PPTAG_CHARACTER_MONSTER))
		{
			UAISense_Damage::ReportDamageEvent(this, HitResult.GetActor(), OwnerASC->GetAvatarActor(),
				OwnerAttributeSet->GetAttackRate(), HitResult.GetActor()->GetActorLocation(), HitResult.Location);
		}
	}
}

void UPPGA_AttackHitCheck::ClientPlayHitCue(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	PPNET_SUBLOG(LogGAS, Log, TEXT("Begin"));

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(DataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(DataHandle, 0);

		UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		if (!OwnerASC || !TargetASC)
		{
			PPGAS_LOG(LogGAS, Error, TEXT("(Owner or Target)ASC Not Found"));
			return;
		}

		//Hit 이벤트 발생
		FGameplayEventData PayLoadData;
		PayLoadData.Instigator = OwnerASC->GetAvatarActor();
		PayLoadData.Target = TargetASC->GetAvatarActor();
		OwnerASC->HandleGameplayEvent(PPTAG_ABILITY_HIT, &PayLoadData);
	}
}
