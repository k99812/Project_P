// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "PPAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPGruntAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//#include "AIController.h" 추가
	//BTTask의 OwnerComp(BT)에서 GetAIOwner로 BT를 소유하고있는 AI컨트롤러를 가져와 AI컨트롤러를 소유하고있는 폰을 가져옴
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControlPawn))
	{
		return EBTNodeResult::Failed;
	}

	//#include "NavigationSystem.h" 추가
	//ControlPawn을 이용해 월드를 가져와 월드에서 UNavigationSystemV1을 가져옴
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControlPawn->GetWorld());
	if (!IsValid(NavSystem))
	{
		return EBTNodeResult::Failed;
	}

	//블랙보드에 있는 데이터 가져오기
	//#include "BehaviorTree/BlackboardComponent.h" 추가
	FVector OriginPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;
	float NextPosRadius;

	//#include "AbilitySystemComponent.h", #include "Attribute/PPGruntAttributeSet.h"
	//#include "AbilitySystemBlueprintLibrary.h" 추가
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlPawn);
	if (ASC)
	{
		const UPPGruntAttributeSet* AttributeSet = ASC->GetSet<UPPGruntAttributeSet>();
		if (AttributeSet)
		{
			NextPosRadius = AttributeSet->GetAIPatrolRadius();
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	//GetRandomPointInNavigableRadius(기준 위치, 반지름(반경), 위치를 받아올 FNavLocation)
	if (NavSystem->GetRandomPointInNavigableRadius(OriginPos, NextPosRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
