// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPGASCharacterNonPlayer.h"
#include "InterFace/PPMonsterInterface.h"
#include "PPGASCharacterGrunt.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGASCharacterGrunt : public APPGASCharacterNonPlayer, public IPPMonsterInterface
{
	GENERATED_BODY()
	
public:
	APPGASCharacterGrunt();

	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetMonstHpBarVisibility(bool bVisible) override;

public:
	virtual void SetDead() override;

	UFUNCTION()
	void FoundTargetCallback(bool bFoundTarget, AActor* TargetActor);

protected:
	UPROPERTY(EditAnywhere, Category = "AttributeData")
	TObjectPtr<class UPPGruntAttributeData> AttributeData;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UPPGASWidgetComponent> HpBar;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> InitStatEffect;

// Damage
protected:
	virtual void TakeDamage(const FOnAttributeChangeData& ChangeData) override;
};
