// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/PPAT_Trace.h"
#include "GA/TA/PPTA_Trace.h"
#include "AbilitySystemComponent.h"

UPPAT_Trace::UPPAT_Trace()
{
}

UPPAT_Trace* UPPAT_Trace::CreateTask(UGameplayAbility* OwnerAbility, TSubclassOf<class APPTA_Trace> TargetActorClass)
{
	UPPAT_Trace* NewTask = NewAbilityTask<UPPAT_Trace>(OwnerAbility);

	NewTask->TargetActorClass = TargetActorClass;

	return NewTask;
}

void UPPAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UPPAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UPPAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<APPTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity,
		nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UPPAT_Trace::TargetDataReadyCallback);
	}
}

void UPPAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UPPAT_Trace::TargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
