// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAttaking.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UBTDecorator_IsAttaking : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsAttaking();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
