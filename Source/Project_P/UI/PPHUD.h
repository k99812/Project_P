// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PPHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	APPHUD();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> StatBar;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UUserWidget> StatBarRef;
};
