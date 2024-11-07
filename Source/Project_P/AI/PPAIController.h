// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PPAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APPAIController();

	void RunAI();
	void StopAI();

protected:
	//��Ʈ�ѷ��� ���� �����Ҷ� ����Ǵ� �Լ�
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
