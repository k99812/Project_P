// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/PPGA_Attack.h"
#include "Tag/PPGameplayTag.h"
#include "GameplayTagContainer.h"
#include "Input/PPInputEnum.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Retsult = Super::ExecuteTask(OwnerComp, NodeMemory);

	//#include "AIController.h" Ãß°¡
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

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID((int32)EInputAbility::LeftAttack);
	if (Spec)
	{
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControllingPawn))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
	if (!IsValid(ASC))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FGameplayTagContainer Tags(PPTAG_ABILITY_ATTACK);
	if (!ASC->HasAllMatchingGameplayTags(Tags))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
