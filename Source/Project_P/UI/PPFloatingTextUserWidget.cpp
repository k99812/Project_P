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

	
}

void UPPFloatingTextUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UPPFloatingTextUserWidget::SetTextWidget(const float& Damage, const FVector& ActorPosition)
{
	FirstLocation = ActorPosition;

	float Random_X = FMath::RandRange(-150, 150);
	float Random_Y = FMath::RandRange(-200, -100);

	LastLocation = ActorPosition;
	LastLocation.X += Random_X;
	LastLocation.Y += Random_Y;

	DamageTxt->SetText(FText::FromString(FString::Printf(TEXT("%f"), Damage)));
}
