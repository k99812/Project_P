// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPHUD.h"
#include "UI/PPGASPlayerHpBarUserWidget.h"

APPHUD::APPHUD()
{
	//UI
	static ConstructorHelpers::FClassFinder<UUserWidget> StatBarClassRef(TEXT("/Game/Project_P/Blueprint/UI/WBP_PlayerHpBar.WBP_PlayerHpBar_C"));
	if (StatBarClassRef.Class)
	{
		StatBar = StatBarClassRef.Class;
	}
}

void APPHUD::BeginPlay()
{
	if (StatBar)
	{
		StatBarRef = CreateWidget<UUserWidget>(GetWorld(), StatBar);
	}
	
	if (StatBarRef)
	{
		StatBarRef->AddToViewport();
	}
}
