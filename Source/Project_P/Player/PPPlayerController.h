// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APPPlayerController();

protected:
	virtual void BeginPlay() override;

// HUD
protected:
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<class UPPHUDWidget> HUDWidget;
};
