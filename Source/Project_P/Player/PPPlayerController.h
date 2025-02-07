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

	//블루프린트 임플리먼트 이벤트는 함수 본문 구현을 언리얼에서 자동으로 해줌
	UFUNCTION(BlueprintImplementableEvent, Category = "Game", Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	void GameOver();

protected:
	virtual void BeginPlay() override;

// HUD
protected:
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<class UPPHUDWidget> HUDWidget;

//GameOver UI
protected:
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> GameOverUIClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<class UPPGameOverUserWidget> GameOverUIWidget;
};
