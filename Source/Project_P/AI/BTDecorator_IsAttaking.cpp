// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsAttaking.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Tag/PPGameplayTag.h"

UBTDecorator_IsAttaking::UBTDecorator_IsAttaking()
{
	NodeName = TEXT("IsAttacking");
}

bool UBTDecorator_IsAttaking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControllingPawn))
	{
		return false;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
	if (!IsValid(ASC))
	{
		return false;
	}

	FGameplayTagContainer Tags(PPTAG_CHARACTER_ISATTACKING);
	bResult = ASC->HasAllMatchingGameplayTags(Tags);

	return bResult;
}
