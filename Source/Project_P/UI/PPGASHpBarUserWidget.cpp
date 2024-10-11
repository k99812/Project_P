// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGASHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPPGASHpBarUserWidget::SetAbilitySystemComponent(AActor* Owner)
{
	Super::SetAbilitySystemComponent(Owner);

	if (ASC)
	{
		//Ư�� ��Ʈ����Ʈ���� �ٲ� ���� ȣ��Ǵ� ��������Ʈ
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UPPGASHpBarUserWidget::OnHealthAttributeChange);
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UPPGASHpBarUserWidget::OnMaxHealthAttributeChange);
	
		const UPPCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UPPCharacterAttributeSet>();
		ensure(CurrentAttributeSet);

		//CurrentHealth = CurrentAttributeSet->GetHealth();
		//CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
		//ensure(CurrentMaxHealth > 0.0f);

		UpdateHpBar();
	}
}

void UPPGASHpBarUserWidget::OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UPPGASHpBarUserWidget::OnMaxHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UPPGASHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}