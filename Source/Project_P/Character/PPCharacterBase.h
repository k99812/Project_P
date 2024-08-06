// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPCharacterBase.generated.h"

UCLASS()
class PROJECT_P_API APPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPCharacterBase();

	FORCEINLINE TArray<TObjectPtr<UAnimMontage>> GetComboAttackMontage() { return ComboAttackMontage; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> ComboAttackMontage;
};
