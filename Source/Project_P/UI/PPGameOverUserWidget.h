// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPGameOverUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPGameOverUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BtnEventGameRestart();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BtnRestart;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtGameOver;
};
