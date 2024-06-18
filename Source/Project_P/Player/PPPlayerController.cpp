// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPPlayerController.h"

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//게임 시작하면 마우스 가두기
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
