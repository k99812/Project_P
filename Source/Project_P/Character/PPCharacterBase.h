// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPCharacterBase.generated.h"

DECLARE_DELEGATE(FInputReleasedDelegate);

UCLASS()
class PROJECT_P_API APPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPCharacterBase();

	FORCEINLINE TObjectPtr<UAnimMontage> GetComboAttackMontage() { return ComboAttackMontage; }
	FORCEINLINE TObjectPtr<class UPPComboActionData> GetComboActionData() { return ComboActionData; }

	// InputRelease Delegate
public:
	FInputReleasedDelegate InputReleasedDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<class UPPComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

// Dead Section
protected:
	virtual void SetDead();
	virtual void PlayDeadAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = "true"))
	float DeadEventDelayTime = 5.0f;
};
