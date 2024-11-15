// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "PPAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


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
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControlPawn->GetWorld());
	if (!IsValid(NavSystem))
	{
		return EBTNodeResult::Failed;
	}

	//블랙보드에 있는 데이터 가져오기
	FVector OriginPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);

	return Result;
}
