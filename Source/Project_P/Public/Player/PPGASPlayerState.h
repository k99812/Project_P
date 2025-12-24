// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Interface/PPPlayerStateInterface.h"
#include "PPGASPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGASPlayerState : public APlayerState, public IAbilitySystemInterface, public IPPPlayerStateInterface
{
	GENERATED_BODY()
	
public:
	APPGASPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE virtual bool GetIsAbilitiesGiven() const override { return bIsAbilitiesGiven; }
	FORCEINLINE virtual void SetIsAbilitiesGiven(bool bGiven) override { bIsAbilitiesGiven = bGiven; }

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category = AttributeSet)
	TObjectPtr<class UPPCharacterAttributeSet> AttributeSet;

	bool bIsAbilitiesGiven = false;
};
