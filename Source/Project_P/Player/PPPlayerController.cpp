// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PPPlayerController.h"

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//���� �����ϸ� ���콺 ���α�
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
