// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPFloatingTextUserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Subsystem/PPDamageUISubsystem.h"

UPPFloatingTextUserWidget::UPPFloatingTextUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPFloatingTextUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AnimFinishedDelegate.BindDynamic(this, &UPPFloatingTextUserWidget::AnimationFinished);
	BindToAnimationFinished(FadeOut, AnimFinishedDelegate);
}

bool UPPFloatingTextUserWidget::SetTextWidget(const float Damage, const FVector& ActorPosition)
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController) return false;

	FVector2D WidgetLocation;
	if (PlayerController->ProjectWorldLocationToScreen(ActorPosition, WidgetLocation))
	{
		SetPositionInViewport(WidgetLocation);
	}
	else
	{
		return false;
	}

	if (DamageTxt)
	{
		DamageTxt->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Damage)));
		DamageTxt->SetRenderTranslation(FVector2D::ZeroVector);
	}
	
	SetRenderOpacity(1.0f);
	SetVisibility(ESlateVisibility::HitTestInvisible);

	if (FadeOut)
	{
		PlayAnimation(FadeOut, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}

	if (TextUp)
	{
		PlayAnimation(TextUp, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}

	return true;
}

void UPPFloatingTextUserWidget::AnimationFinished()
{
	//EndLifeTime.ExecuteIfBound();
	UPPDamageUISubsystem* DamgaeUISubsystem = GetWorld()->GetSubsystem<UPPDamageUISubsystem>();
	if (DamgaeUISubsystem)
	{
		DamgaeUISubsystem->ReturnToPool(this);
	}
}
