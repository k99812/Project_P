// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "PPGruntAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGruntAttributeSet : public UPPCharacterAttributeSet
{
	GENERATED_BODY()
	
public:
	UPPGruntAttributeSet();

	ATTRIBUTE_ACCESSORS(UPPGruntAttributeSet, AIPatrolRadius);
	ATTRIBUTE_ACCESSORS(UPPGruntAttributeSet, AIDetectRadius);
	ATTRIBUTE_ACCESSORS(UPPGruntAttributeSet, AILoseRadius);
	ATTRIBUTE_ACCESSORS(UPPGruntAttributeSet, AIVisionAngleDeg);
	ATTRIBUTE_ACCESSORS(UPPGruntAttributeSet, AITurnSpeed);
	ATTRIBUTE_ACCESSORS(UPPGruntAttributeSet, AISenseAge);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AIPatrolRadius;

	UPROPERTY(BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AIDetectRadius;

	UPROPERTY(BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AILoseRadius;

	UPROPERTY(BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AIVisionAngleDeg;

	UPROPERTY(BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AITurnSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "AI", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AISenseAge;
};