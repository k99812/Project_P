// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PPGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "PPGASHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGASHpBarUserWidget : public UPPGASUserWidget
{
	GENERATED_BODY()

protected:
	virtual void SetAbilitySystemComponent(AActor* Owner) override;

	//#include "GameplayEffectTypes.h"�߰�
	void OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthAttributeChange(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();

	//meta = (BindWidget) ������ ������ �̸��� ���� ������ ã�� ������ �ʱ�ȭ ����
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;
};
