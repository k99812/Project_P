// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_GASAttackHitCheck::UAnimNotify_GASAttackHitCheck()
{
}

FString UAnimNotify_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	return FString(TEXT("GASAttackHitCheck"));
}

void UAnimNotify_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwerActor = MeshComp->GetOwner();
		if (OwerActor)
		{
			FGameplayEventData PayLoadData;

			//#include "AbilitySystemBlueprintLibrary.h" �߰�
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwerActor, TriggerGameplayTag, PayLoadData);
		}
	}
}