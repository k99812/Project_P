// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "PPGASCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGASCharacterNonPlayer : public APPCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APPGASCharacterNonPlayer();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* NewController) override;

// GAS Section
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category = AttributeSet)
	TObjectPtr<class UPPGruntAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilites;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<EInputAbility, TSubclassOf<class UGameplayAbility>> StartInputAbilites;

// Dead Section
protected:
	UFUNCTION()
	virtual void ActorIsDead();

	virtual void SetDead() override;
};
