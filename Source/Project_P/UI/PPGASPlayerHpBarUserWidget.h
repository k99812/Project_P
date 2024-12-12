// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PPGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "PPGASPlayerHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGASPlayerHpBarUserWidget : public UPPGASUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* Owner) override;

	void OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthAttributeChange(const FOnAttributeChangeData& ChangeData);

	void OnManaAttributeChange(const FOnAttributeChangeData& ChangeData);
	void OnMaxManaAttributeChange(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();
	void UpdateMpBar();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbMpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtMpStat;
};
