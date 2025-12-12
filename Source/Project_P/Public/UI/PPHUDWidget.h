// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPPHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPPPlayerStatBarUserWidget> PlayerStatBar;
};
