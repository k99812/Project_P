// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PPGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "PPPlayerStatBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPPlayerStatBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

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

private:
	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

	float CurrentMana = 0.0f;
	float CurrentMaxMana = 0.1f;
};
