// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPFloatingTextUserWidget.generated.h"

DECLARE_DELEGATE(FEndLifeTime)

/**
 * 
 */
UCLASS()
class PROJECT_P_API UPPFloatingTextUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPPFloatingTextUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetTextWidget(const float& Damage, const FVector& ActorPosition);

	FEndLifeTime EndLifeTime;

//Widget Property
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageTxt;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> FadeOut;

//Location
protected:
	FVector FirstLocation;

	FVector LastLocation;
};
