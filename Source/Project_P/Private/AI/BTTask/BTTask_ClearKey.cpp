// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_ClearKey.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearKey::UBTTask_ClearKey()
{
	NodeName = TEXT("Clear Blackboard Key");
}

EBTNodeResult::Type UBTTask_ClearKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (Result != EBTNodeResult::Succeeded)
	{
		return Result;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (Blackboard)
	{
		Blackboard->ClearValue(GetSelectedBlackboardKey());

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
