// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PPGruntAttributeData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGruntAttributeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPPGruntAttributeData();

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float AttackRange;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxAttackRange;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float AttackRadius;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxAttackRadius;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float AttackRate;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxAttackRate;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float AIPatrolRadius;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float AIDetectRange;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float AITurnSpeed;
};
