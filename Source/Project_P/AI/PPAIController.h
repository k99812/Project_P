// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
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

	virtual void BeginPlay() override;

//AI Section
protected:
	UFUNCTION()
	void ActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void ActorPerceptionForgetUpdated(AActor* Actor);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAISenseConfig_Sight> SenseConfig_Sight;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAISenseConfig_Hearing> SenseConfig_Hearing;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAISenseConfig_Damage> SenseConfig_Damage;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<class UPPGruntAIData> GruntAIData;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<APawn> ControlledPawn;
};
