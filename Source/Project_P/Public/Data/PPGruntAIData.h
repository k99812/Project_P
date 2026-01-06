// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PPGruntAIData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGruntAIData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPPGruntAIData();

	UPROPERTY(EditAnywhere, Category = "Data")
	float SightRadius;

	UPROPERTY(EditAnywhere, Category = "Data")
	float LoseSightRadius;

	UPROPERTY(EditAnywhere, Category = "Data")
	float HearingRange;

	UPROPERTY(EditAnywhere, Category = "Data")
	float HearingLoudness;

	UPROPERTY(EditAnywhere, Category = "Data")
	float AIVisionAngleDeg;

	UPROPERTY(EditAnywhere, Category = "Data")
	float AISenseAge;

	UPROPERTY(EditAnywhere, Category = "Data")
	float AutoSuccessRange;
};
