// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "PPGASCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGASCharacterPlayer : public APPCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APPGASCharacterPlayer();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;

// Ability System Section
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
};
