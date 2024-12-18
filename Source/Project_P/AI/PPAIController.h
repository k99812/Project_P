// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PPAIController.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FFindTargetDelegate, bool, bFindTarget);

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

	FFindTargetDelegate FindTargetDelegate;

protected:
	//컨트롤러가 폰에 빙의할때 실행되는 함수
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

//AI Section
protected:
	UFUNCTION()
	virtual void ActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	virtual void ActorPerceptionForgetUpdated(AActor* Actor);

	virtual void PerceptionSensedSight(APawn* Pawn_);

	virtual void PerceptionSensedHearing(APawn* Pawn_);

	virtual void PerceptionSensedDamage(APawn* Pawn_);

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

	void ResetTarget();
};
