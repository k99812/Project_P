// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPFloatingTextUserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

UPPFloatingTextUserWidget::UPPFloatingTextUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPFloatingTextUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AnimFinishedDelegate.BindDynamic(this, &UPPFloatingTextUserWidget::AnimationFinished);
	BindToAnimationFinished(FadeOut, AnimFinishedDelegate);
	
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPPFloatingTextUserWidget::AnimationFinished, FadeOut->GetEndTime(), false);

	SetPositionInViewport(FirstLocation);
	PlayAnimation(FadeOut);
	PlayAnimation(TextUp);
}

bool UPPFloatingTextUserWidget::SetTextWidget(const float& Damage, const FVector& ActorPosition)
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)
	{
		return false;
	}

	if (!PlayerController->ProjectWorldLocationToScreen(ActorPosition, FirstLocation))
	{
		return false;
	}

	DamageTxt->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Damage)));
	
	return true;
}

void UPPFloatingTextUserWidget::AnimationFinished()
{
	TimerHandle.Invalidate();
	EndLifeTime.ExecuteIfBound();
}
