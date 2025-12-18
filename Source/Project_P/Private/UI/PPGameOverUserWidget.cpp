// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGameOverUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Interface/PPPlayerInterface.h"
#include "Player/PPGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PPCharacterAttributeSet.h"
#include "Tag/PPGameplayTag.h"


UPPGameOverUserWidget::UPPGameOverUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPGameOverUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//BtnRestart->OnClicked.AddDynamic(this, &UPPGameOverUserWidget::BtnEventGameRestart);

	PlayAnimation(Color);
}

void UPPGameOverUserWidget::BtnEventGameRestart()
{
	APlayerController* OwingPlayerController = GetOwningPlayer();
	IPPPlayerInterface* Player = Cast<IPPPlayerInterface>(OwingPlayerController);
	
	if (!OwingPlayerController) UE_LOG(LogTemp, Error, TEXT("RestartBtn: PC is NULL"));
	if (!Player) UE_LOG(LogTemp, Error, TEXT("RestartBtn: Interface Cast Failed"));

	if (OwingPlayerController && Player)
	{
		APPGASPlayerState* PlayerState = OwingPlayerController->GetPlayerState<APPGASPlayerState>();
		if (PlayerState)
		{
			UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();

			if (ASC)
			{
				if (ASC->HasMatchingGameplayTag(PPTAG_CHARACTER_ISDEAD))
				{
					ASC->RemoveLooseGameplayTag(PPTAG_CHARACTER_ISDEAD);
				}

				if (const UPPCharacterAttributeSet* AttributeSet = ASC->GetSet<UPPCharacterAttributeSet>())
				{
					const_cast<UPPCharacterAttributeSet*>(AttributeSet)->SetIsDead(false);
				}
			}
		}

		Player->RequestRespawn();

		OwingPlayerController->SetShowMouseCursor(false);
		OwingPlayerController->SetInputMode(FInputModeGameOnly());
	}

	RemoveFromParent();
}