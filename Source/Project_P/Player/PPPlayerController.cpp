// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPPlayerController.h"
#include "UI/PPHUDWidget.h"
#include "UI/PPGameOverUserWidget.h"

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
