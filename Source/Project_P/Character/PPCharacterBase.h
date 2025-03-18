// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "Input/PPInputEnum.h"
#include "GenericTeamAgentInterface.h"
#include "PPCharacterBase.generated.h"

UCLASS()
class PROJECT_P_API APPCharacterBase : public ACharacter, public IGameplayTagAssetInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPCharacterBase();

	FORCEINLINE TObjectPtr<UAnimMontage> GetComboAttackMontage() { return ComboAttackMontage; }
	FORCEINLINE TObjectPtr<class UPPComboActionData> GetComboActionData() { return ComboActionData; }

	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<class UPPComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", Meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTags;

// Dead Section
protected:
	virtual void SetDead();
	virtual void PlayDeadAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = "true"))
	float DeadEventDelayTime = 5.0f;

//GenericTeam Section
public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	FGenericTeamId GenericTeamId;
};
