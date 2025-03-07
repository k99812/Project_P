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

	UE_LOG(LogTemp, Log, TEXT("DamageUI NativeConstruct"));

	AnimFinishedDelegate.BindDynamic(this, &UPPFloatingTextUserWidget::AnimationFinished);
	BindToAnimationFinished(FadeOut, AnimFinishedDelegate);

	//SetPositionInViewport(FirstLocation);
	PlayAnimation(FadeOut);
}

void UPPFloatingTextUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//SetPositionInViewport(FMath::Vector2DInterpConstantTo(FirstLocation, LastLocation, InDeltaTime, 1.f));
}

void UPPFloatingTextUserWidget::SetTextWidget(const float& Damage, const FVector& ActorPosition)
{
	FirstLocation = FVector2D(ActorPosition);

	float Random_X = FMath::RandRange(-150, 150);
	float Random_Y = FMath::RandRange(-200, -100);

	LastLocation = FVector2D(ActorPosition.X + Random_X, ActorPosition.Y + Random_Y);

	DamageTxt->SetText(FText::FromString(FString::Printf(TEXT("%f"), Damage)));
}

void UPPFloatingTextUserWidget::AnimationFinished()
{
	UE_LOG(LogTemp, Log, TEXT("DamageUI Remove To Viewport"));
	EndLifeTime.ExecuteIfBound();
	//RemoveFromParent();
}
