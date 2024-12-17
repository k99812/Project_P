// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPPlayerStatBarUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UPPPlayerStatBarUserWidget::UPPPlayerStatBarUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPPlayerStatBarUserWidget::NativeConstruct()
{
	UE_LOG(LogTemp, Log, TEXT("HUD : %s"), *GetOwningPlayerPawn()->GetName());

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetHealthAttribute()).
			AddUObject(this, &UPPPlayerStatBarUserWidget::OnHealthAttributeChange);
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetMaxHealthAttribute()).
			AddUObject(this, &UPPPlayerStatBarUserWidget::OnMaxHealthAttributeChange);

		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetManaAttribute()).
			AddUObject(this, &UPPPlayerStatBarUserWidget::OnManaAttributeChange);
		ASC->GetGameplayAttributeValueChangeDelegate(UPPCharacterAttributeSet::GetMaxManaAttribute()).
			AddUObject(this, &UPPPlayerStatBarUserWidget::OnMaxManaAttributeChange);

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

void UPPPlayerStatBarUserWidget::OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UPPPlayerStatBarUserWidget::OnMaxHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UPPPlayerStatBarUserWidget::OnManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateMpBar();
}

void UPPPlayerStatBarUserWidget::OnMaxManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxMana = ChangeData.NewValue;
	UpdateMpBar();
}

void UPPPlayerStatBarUserWidget::UpdateHpBar()
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

void UPPPlayerStatBarUserWidget::UpdateMpBar()
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
