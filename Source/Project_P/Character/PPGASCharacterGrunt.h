// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPGASCharacterNonPlayer.h"
#include "PPGASCharacterGrunt.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGASCharacterGrunt : public APPGASCharacterNonPlayer
{
	GENERATED_BODY()
	
public:
	APPGASCharacterGrunt();

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditAnywhere, Category = "AttributeData")
	TObjectPtr<class UPPGruntAttributeData> AttributeData;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UPPGASWidgetComponent> HpBar;
};
