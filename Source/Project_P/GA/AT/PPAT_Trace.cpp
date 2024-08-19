// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/PPAT_Trace.h"

UPPAT_Trace::UPPAT_Trace()
{
}

UPPAT_Trace* UPPAT_Trace::CreateTask(UGameplayAbility* OwnerAbility)
{
	UPPAT_Trace* NewTask = NewAbilityTask<UPPAT_Trace>(OwnerAbility);

	return NewTask;
}

void UPPAT_Trace::Activate()
{
	Super::Activate();

	
}

void UPPAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UPPAT_Trace::TargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
