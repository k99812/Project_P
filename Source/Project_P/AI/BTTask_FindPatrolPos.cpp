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

	//#include "AIController.h" �߰�
	//BTTask�� OwnerComp(BT)���� GetAIOwner�� BT�� �����ϰ��ִ� AI��Ʈ�ѷ��� ������ AI��Ʈ�ѷ��� �����ϰ��ִ� ���� ������
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControlPawn))
	{
		return EBTNodeResult::Failed;
	}

	//#include "NavigationSystem.h" �߰�
	//ControlPawn�� �̿��� ���带 ������ ���忡�� UNavigationSystemV1�� ������
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControlPawn->GetWorld());
	if (!IsValid(NavSystem))
	{
		return EBTNodeResult::Failed;
	}

	//�����忡 �ִ� ������ ��������
	//#include "BehaviorTree/BlackboardComponent.h" �߰�
	FVector OriginPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;
	float NextPosRadius;

	//#include "AbilitySystemComponent.h", #include "Attribute/PPGruntAttributeSet.h"
	//#include "AbilitySystemBlueprintLibrary.h" �߰�
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

	//GetRandomPointInNavigableRadius(���� ��ġ, ������(�ݰ�), ��ġ�� �޾ƿ� FNavLocation)
	if (NavSystem->GetRandomPointInNavigableRadius(OriginPos, NextPosRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
