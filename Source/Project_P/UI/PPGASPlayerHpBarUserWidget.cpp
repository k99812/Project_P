// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGASPlayerHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPPGASPlayerHpBarUserWidget::SetAbilitySystemComponent(AActor* Owner)
{
	Super::SetAbilitySystemComponent(Owner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetHealthAttribute()).
			AddUObject(this, &UPPGASPlayerHpBarUserWidget::OnHealthAttributeChange);
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetMaxHealthAttribute()).
			AddUObject(this, &UPPGASPlayerHpBarUserWidget::OnMaxHealthAttributeChange);

		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetManaAttribute()).
			AddUObject(this, &UPPGASPlayerHpBarUserWidget::OnManaAttributeChange);
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetMaxManaAttribute()).
			AddUObject(this, &UPPGASPlayerHpBarUserWidget::OnMaxManaAttributeChange);

		const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>();
		if (AttributeSet)
		{
			CurrentHealth = AttributeSet->GetHealth();
			CurrentMaxHealth = AttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}

			CurrentMana = AttributeSet->GetMana();
			CurrentMaxMana = AttributeSet->GetMaxMana();

			if (CurrentMaxMana > 0.0f)
			{
				UpdateMpBar();
			}
		}
	}
}

void UPPGASPlayerHpBarUserWidget::OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UPPGASPlayerHpBarUserWidget::OnMaxHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UPPGASPlayerHpBarUserWidget::OnManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateMpBar();
}

void UPPGASPlayerHpBarUserWidget::OnMaxManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxMana = ChangeData.NewValue;
	UpdateMpBar();
}

void UPPGASPlayerHpBarUserWidget::UpdateHpBar()
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

void UPPGASPlayerHpBarUserWidget::UpdateMpBar()
{
	if (PbMpBar)
	{
		PbMpBar->SetPercent(CurrentMana / CurrentMaxMana);
	}

	if (TxtHpStat)
	{
		TxtMpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentMana, CurrentMaxMana)));
	}
}
