// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_TurnTo.h"
#include "AI/PPAIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/PPGruntAttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnTo::UBTTask_TurnTo()
{
	NodeName = TEXT("TurnToController");
}

EBTNodeResult::Type UBTTask_TurnTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Retsult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControllingPawn))
	{
		return EBTNodeResult::Type::Failed;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControllingPawn);
	if (!IsValid(ASC))
	{
		return EBTNodeResult::Type::Failed;
	}

	const UPPGruntAttributeSet* AttributeSet = ASC->GetSet<UPPGruntAttributeSet>();
	if (!IsValid(AttributeSet))
	{
		return EBTNodeResult::Type::Failed;
	}

	FVector TargetPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	FVector NowPos = ControllingPawn->GetActorLocation();
	FVector LookVector = TargetPos - NowPos;
	FRotator LookRotation = LookVector.Rotation();

	OwnerComp.GetAIOwner()->SetControlRotation(LookRotation);
	Retsult = EBTNodeResult::Type::Succeeded;

	return Retsult;
}
