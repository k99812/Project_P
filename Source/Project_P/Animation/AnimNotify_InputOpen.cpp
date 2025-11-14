// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_InputOpen.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Project_P.h"

UAnimNotify_InputOpen::UAnimNotify_InputOpen()
{

}

FString UAnimNotify_InputOpen::GetNotifyName_Implementation() const
{
	return FString(TEXT("NotifyInputOpen"));
}

void UAnimNotify_InputOpen::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwerActor = MeshComp->GetOwner();
		if (OwerActor)
		{			
			PPNET_ANIMLOG(LogGAS, Log, TEXT("Begin"));
			FGameplayEventData PayLoadData;

			//#include "AbilitySystemBlueprintLibrary.h" Ãß°¡
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwerActor, TriggerGameplayTag, PayLoadData);
		}
	}
}
