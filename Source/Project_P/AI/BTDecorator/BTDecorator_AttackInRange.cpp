// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_AttackInRange.h"
#include "AI/PPAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPGruntAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	//데코레이터 노드 이름 지정
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//#include "AIController.h" 추가
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControllingPawn))
	{
		return false;
	}

	//#include "AbilitySystemComponent.h", #include "AbilitySystemBlueprintLibrary.h" 추가
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
	if (!IsValid(ASC))
	{
		return false;
	}

	//#include "Attribute/PPGruntAttributeSet.h" 추가
	const UPPGruntAttributeSet* AttributeSet = ASC->GetSet<UPPGruntAttributeSet>();
	if (!IsValid(AttributeSet))
	{
		return false;
	}

	//#include "BehaviorTree/BlackboardComponent.h" 추가
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!IsValid(Target))
	{
		return false;
	}

	float AttackRange = AttributeSet->GetBTAttackRange();
	bResult = ControllingPawn->GetDistanceTo(Target) <= AttackRange;

	return bResult;
}
