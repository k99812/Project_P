// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "UI/PPHUDWidget.h"
#include "UI/PPGameOverUserWidget.h"
#include "UI/PPFloatingTextUserWidget.h"
#include "Tag/PPGameplayTag.h"
#include "Project_P.h"


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

	if (GameOverUIWidget && GameOverUIWidget->IsInViewport()) return;

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
	IAbilitySystemInterface* IPlayerState = Cast<IAbilitySystemInterface>(PlayerState);
	UAbilitySystemComponent* ASC = IPlayerState ? IPlayerState->GetAbilitySystemComponent() : nullptr;

	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD))
		{
			ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
		}

		ASC->CurrentMontageStop(0.0f);

		if (const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>())
		{
			const_cast<UPPCharacterAttributeSet*>(AttributeSet)->SetIsDead(false);
		}
	}

	ACharacter* OldCharacter = Cast<ACharacter>(GetPawn());
	if (OldCharacter)
	{
		if (OldCharacter->GetMesh() && OldCharacter->GetMesh()->GetAnimInstance())
		{
			OldCharacter->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
		}

		UnPossess();
		OldCharacter->Destroy();
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