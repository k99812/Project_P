// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPPlayerStatBarUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Project_P.h"

UPPPlayerStatBarUserWidget::UPPPlayerStatBarUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPPlayerStatBarUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UPPPlayerStatBarUserWidget::BindAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (!ASC)
	{
		PPGAS_LOG(LogGAS, Warning, TEXT("ASC doesn't exit"));
		return;
	}

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
		CurrentHealth = TargetHealth = AttributeSet->GetHealth();
		CurrentMaxHealth = TargetMaxHealth = AttributeSet->GetMaxHealth();

		if (CurrentMaxHealth > 0.0f)
		{
			UpdateHpBar();
		}

		CurrentMana = TargetMana = AttributeSet->GetMana();
		CurrentMaxMana = TargetMaxMana = AttributeSet->GetMaxMana();

		if (CurrentMaxMana > 0.0f)
		{
			UpdateMpBar();
		}
	}
}

void UPPPlayerStatBarUserWidget::OnHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	TargetHealth = ChangeData.NewValue;
	//UpdateHpBar();
	CheckShouldTick();
}

void UPPPlayerStatBarUserWidget::OnMaxHealthAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	TargetMaxHealth = ChangeData.NewValue;
	//UpdateHpBar();
	CheckShouldTick();
}

void UPPPlayerStatBarUserWidget::OnManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	TargetMana = ChangeData.NewValue;
	//UpdateMpBar();
	CheckShouldTick();
}

void UPPPlayerStatBarUserWidget::OnMaxManaAttributeChange(const FOnAttributeChangeData& ChangeData)
{
	TargetMaxMana = ChangeData.NewValue;
	//UpdateMpBar();
	CheckShouldTick();
}

void UPPPlayerStatBarUserWidget::CheckShouldTick()
{
	bool bHealthMatched = FMath::IsNearlyEqual(CurrentHealth, TargetHealth, 0.1f) &&
		FMath::IsNearlyEqual(CurrentMaxHealth, TargetMaxHealth, 0.1f);

	bool bManaMatched = FMath::IsNearlyEqual(CurrentMana, TargetMana, 0.1f) &&
		FMath::IsNearlyEqual(CurrentMaxMana, TargetMaxMana, 0.1f);

	if (bHealthMatched && bManaMatched)
	{
		if (InterpTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(InterpTimerHandle);
			InterpTimerHandle.Invalidate();
		}
	}
	else
	{
		if (!InterpTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(InterpTimerHandle, this, &UPPPlayerStatBarUserWidget::UpdateStatBar, TimerFrequency, true);
		}
	}
}

void UPPPlayerStatBarUserWidget::UpdateStatBar()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!FMath::IsNearlyEqual(CurrentHealth, TargetHealth, 0.1f) ||
		!FMath::IsNearlyEqual(CurrentMaxHealth, TargetMaxHealth, 0.1f))
	{
		CurrentHealth = FMath::FInterpTo(CurrentHealth, TargetHealth, DeltaTime, BarInterpSpeed);
		CurrentMaxHealth = FMath::FInterpTo(CurrentMaxHealth, TargetMaxHealth, DeltaTime, BarInterpSpeed);
		UpdateHpBar();
	}

	if (!FMath::IsNearlyEqual(CurrentMana, TargetMana, 0.1f) ||
		!FMath::IsNearlyEqual(CurrentMaxMana, TargetMaxMana, 0.1f))
	{
		CurrentMana = FMath::FInterpTo(CurrentMana, TargetMana, DeltaTime, BarInterpSpeed);
		CurrentMaxMana = FMath::FInterpTo(CurrentMaxMana, TargetMaxMana, DeltaTime, BarInterpSpeed);
		UpdateMpBar();
	}

	CheckShouldTick();
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