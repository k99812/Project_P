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
#include "AbilitySystemInterface.h"


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
	APlayerController* OwningPlayerController = GetOwningPlayer();
	IPPPlayerInterface* IPlayer = Cast<IPPPlayerInterface>(OwningPlayerController);
	
	if (!OwningPlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("RestartBtn: PC is NULL"));
		return;
	}

	if (!IPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("RestartBtn: Interface Cast Failed"));
		return;
	}
	
	IAbilitySystemInterface* IPlayerState = Cast<IAbilitySystemInterface>(OwningPlayerController->PlayerState);
	UAbilitySystemComponent* ASC = IPlayerState ? IPlayerState->GetAbilitySystemComponent() : nullptr;
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

	IPlayer->RequestRespawn();

	OwningPlayerController->SetShowMouseCursor(false);
	OwningPlayerController->SetInputMode(FInputModeGameOnly());

	RemoveFromParent();
}