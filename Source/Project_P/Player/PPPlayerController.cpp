// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPPlayerController.h"
#include "UI/PPHUDWidget.h"
#include "UI/PPGameOverUserWidget.h"
#include "UI/PPFloatingTextUserWidget.h"

APPPlayerController::APPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPPHUDWidget> HUDWidgetClassRef(TEXT("/Game/Project_P/Blueprint/UI/WBP_HUDWidget.WBP_HUDWidget_C"));
	if (HUDWidgetClassRef.Class)
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UPPGameOverUserWidget> GameOverUIClassRef(TEXT("/Game/Project_P/Blueprint/UI/WBP_GameOver.WBP_GameOver_C"));
	if (GameOverUIClassRef.Class)
	{
		GameOverUIClass = GameOverUIClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UPPFloatingTextUserWidget> DamageUIClassRef(TEXT("/Game/Project_P/Blueprint/UI/WBP_FloatingText.WBP_FloatingText_C"));
	if (DamageUIClassRef.Class)
	{
		DamageUIClass = DamageUIClassRef.Class;
	}
}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//게임 시작하면 마우스 가두기
	SetInputMode(FInputModeGameOnly());

	HUDWidget = CreateWidget<UPPHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void APPPlayerController::GameOver()
{
	//K2_OnGameOver();
	GameOverUIWidget = CreateWidget<UPPGameOverUserWidget>(this, GameOverUIClass);
	if (GameOverUIWidget)
	{
		GameOverUIWidget->AddToViewport();
		EnableInput(this);
		SetShowMouseCursor(true);
	}
}

void APPPlayerController::ActorTakedDamage(const float& Damage, const FVector& ActorPosition)
{
	UE_LOG(LogTemp, Log, TEXT("PlayerController Location : %f:%f:%f"), ActorPosition.X, ActorPosition.Y, ActorPosition.Z);

	TWeakObjectPtr<UPPFloatingTextUserWidget> DamageUI = CreateWidget<UPPFloatingTextUserWidget>(this, DamageUIClass);
	if (DamageUI.IsValid())
	{
		DamageUI.Get()->EndLifeTime.BindLambda([&]()
		{
			TWeakObjectPtr<UPPFloatingTextUserWidget> TempDamageUI = DamageUIArray[0];

			if (TempDamageUI.IsValid())
			{
				TempDamageUI.Get()->RemoveFromParent();
			}
			
			DamageUIArray.RemoveAt(0);
		});

		if (DamageUI.Get()->SetTextWidget(Damage, ActorPosition))
		{
			DamageUIArray.Emplace(DamageUI.Get());
			DamageUI.Get()->AddToViewport();
		}
	}
}