// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_GASMoveTo.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"
#include "AIController.h"

UBTTask_GASMoveTo::UBTTask_GASMoveTo()
{
	NodeName = TEXT("GASMoveTo");
}

EBTNodeResult::Type UBTTask_GASMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!IsValid(ControllingPawn))
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
	if (!IsValid(ASC))
	{
		return EBTNodeResult::Failed;
	}

	FGameplayTagContainer WalkingTagContainer;
	WalkingTagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

	if (!ASC->HasAnyMatchingGameplayTags(WalkingTagContainer))
	{
		ASC->AddLooseGameplayTags(WalkingTagContainer);

		//#include "AbilitySystemGlobals.h" 추가
		if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
		{
			ASC->AddReplicatedLooseGameplayTags(WalkingTagContainer);
		}
	}

	//이동시 공격 어빌리티 캔슬
	FGameplayTagContainer CancelAbilityTags(PPTAG_ABILITY_ATTACK);
	ASC->CancelAbilities(&CancelAbilityTags);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTTask_GASMoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
		if (ASC)
		{
			FGameplayTagContainer RemoveTagContainer;
			RemoveTagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

			// ASC에 태그 제거
			if (ASC->HasAnyMatchingGameplayTags(RemoveTagContainer))
			{
				ASC->RemoveLooseGameplayTags(RemoveTagContainer);

				if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
				{
					ASC->RemoveReplicatedLooseGameplayTags(RemoveTagContainer);
				}
			}
		}
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_GASMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
		if (ASC)
		{
			FGameplayTagContainer RemoveTagContainer;
			RemoveTagContainer.AddTag(PPTAG_CHARACTER_ISWALKING);

			// ASC에 태그 제거
			if (ASC->HasAnyMatchingGameplayTags(RemoveTagContainer))
			{
				ASC->RemoveLooseGameplayTags(RemoveTagContainer);

				if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
				{
					ASC->RemoveReplicatedLooseGameplayTags(RemoveTagContainer);
				}
			}
		}
	}

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
