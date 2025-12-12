// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "Input/PPInputEnum.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/PPGASInterface.h"
#include "PPCharacterBase.generated.h"

UCLASS()
class PROJECT_P_API APPCharacterBase : public ACharacter, public IGameplayTagAssetInterface, public IGenericTeamAgentInterface, public IPPGASInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APPCharacterBase();

	FORCEINLINE virtual TObjectPtr<UAnimMontage> GetComboAttackMontage() override { return ComboAttackMontage; }
	FORCEINLINE virtual TObjectPtr<class UPPComboActionData> GetComboActionData() override { return ComboActionData; }
	virtual void Multicast_SendPlayMontage(const FName& Section) override;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayMontage(const FName& Section);

	UFUNCTION(BlueprintCallable, Category = "Tags")
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<class UPPComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags", Meta = (AllowPrivateAccess = "true"))
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
