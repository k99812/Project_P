// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PPPlayerInterface.h"
#include "PPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPPlayerController : public APlayerController, public IPPPlayerInterface
{
	GENERATED_BODY()
	
public:
	APPPlayerController();

// GameOver Function
public:
	//블루프린트 임플리먼트 이벤트는 함수 본문 구현을 언리얼에서 자동으로 해줌
	UFUNCTION(BlueprintImplementableEvent, Category = "Game", Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	void GameOver();

	virtual void RequestRespawn() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_RequestRespawn();

// HUD
protected:
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<class UPPHUDWidget> HUDWidget;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC_Cache;

	virtual void InitHUD(class UAbilitySystemComponent* ASC) override;

//GameOver UI
protected:
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> GameOverUIClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<class UPPGameOverUserWidget> GameOverUIWidget;

	virtual void OnPlayerDead() override;

//Damage UI Section
public:
	void ActorTakedDamage(const float& Damage, const FVector& ActorPosition);

	virtual void RequestShowDamageUI(const float Damage, const FVector& ActorPosition) override;

protected:
	UFUNCTION(Client, Unreliable)
	void ClientRPC_ShowDamageUI(const float Damage, const FVector& ActorPosition);

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UPPFloatingTextUserWidget> DamageUIClass;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
	TArray<TWeakObjectPtr<class UPPFloatingTextUserWidget>> DamageUIArray;
};
