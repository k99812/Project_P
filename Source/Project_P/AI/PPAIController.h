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
	//컨트롤러가 폰에 빙의할때 실행되는 함수
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

//AI Section
protected:
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAISenseConfig_Sight> SenseConfig_Sight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAISenseConfig_Hearing> SenseConfig_Hearing;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAISenseConfig_Damage> SenseConfig_Damage;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<APawn> ControlledPawn;
};
