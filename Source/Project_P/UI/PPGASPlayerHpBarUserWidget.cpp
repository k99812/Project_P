// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGASPlayerHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"

void UPPGASPlayerHpBarUserWidget::SetAbilitySystemComponent(AActor* Owner)
{
	Super::SetAbilitySystemComponent(Owner);

	if (ASC)
	{

		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetHealthAttribute()).
			AddUObject(this, &UPPGASPlayerHpBarUserWidget::OnHealthAttributeChange);

		const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>();
	}
}

void UPPGASPlayerHpBarUserWidget::OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
}

void UPPGASPlayerHpBarUserWidget::OnMaxHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
}

void UPPGASPlayerHpBarUserWidget::OnManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
}

void UPPGASPlayerHpBarUserWidget::OnMaxManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
}

void UPPGASPlayerHpBarUserWidget::UpdateHpBar()
{
}

void UPPGASPlayerHpBarUserWidget::UpdateMpBar()
{
}
