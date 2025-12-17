// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPPlayerController.h"
#include "UI/PPHUDWidget.h"
#include "UI/PPGameOverUserWidget.h"
#include "UI/PPFloatingTextUserWidget.h"
#include "GameFramework/GameModeBase.h"

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

	if (IsLocalPlayerController())
	{
		//게임 시작하면 마우스 가두기
		SetInputMode(FInputModeGameOnly());

		HUDWidget = CreateWidget<UPPHUDWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();

			if (ASC_Cache)
			{
				HUDWidget->BindAbilitySystem(ASC_Cache);
				ASC_Cache = nullptr;
			}
		}
	}
}

void APPPlayerController::InitHUD(UAbilitySystemComponent* ASC)
{
	if (HUDWidget && ASC)
	{
		HUDWidget->BindAbilitySystem(ASC);
	}
	else
	{
		ASC_Cache = ASC;
	}
}

void APPPlayerController::OnPlayerDead()
{
	GameOver();
}

void APPPlayerController::GameOver()
{
	//K2_OnGameOver();
	if (!IsLocalPlayerController()) return;

	GameOverUIWidget = CreateWidget<UPPGameOverUserWidget>(this, GameOverUIClass);
	if (GameOverUIWidget)
	{
		GameOverUIWidget->AddToViewport();
		EnableInput(this);
		SetShowMouseCursor(true);
	}
}

void APPPlayerController::RequestRespawn()
{
	ServerRPC_RequestRespawn();
}

void APPPlayerController::ServerRPC_RequestRespawn_Implementation()
{
	APawn* ControlledPawn = GetPawn();

	if (ControlledPawn)
	{
		UnPossess();

		ControlledPawn->Destroy();
	}

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		GameMode->RestartPlayer(this);
	}
}

void APPPlayerController::ActorTakedDamage(const float& Damage, const FVector& ActorPosition)
{
	if (!IsLocalPlayerController()) return;

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