// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/PPGameInterface.h"
#include "PPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGameMode : public AGameModeBase, public IPPGameInterface
{
	GENERATED_BODY()
	
public:
	APPGameMode();

	virtual void OnPlayerDead() override;

	virtual void OnTakeDamage(const float& Damage, const FVector& ActorPosition) override;
};
