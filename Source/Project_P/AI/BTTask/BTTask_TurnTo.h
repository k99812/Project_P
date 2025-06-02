// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_TurnTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UBTTask_TurnTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_TurnTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
