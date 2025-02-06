// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGameOverUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


UPPGameOverUserWidget::UPPGameOverUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPGameOverUserWidget::NativeConstruct()
{
	BtnRestart->OnClicked.AddDynamic(this, &UPPGameOverUserWidget::BtnEventGameRestart);
}

void UPPGameOverUserWidget::BtnEventGameRestart()
{
	UE_LOG(LogTemp, Log, TEXT("OnCliked BtnEventGameRestart"));

	APlayerController* OwingPlayerController = GetOwningPlayer();

	GetWorld()->Exec(GetWorld(), TEXT("RestartLevel"));
	//OwingPlayerController->ConsoleCommand(TEXT("RestartLevel"));

	//사용자 입력 비활성화
	OwingPlayerController->DisableInput(OwingPlayerController);

	//재시작시 해당 위젯 제거
	RemoveFromParent();
}
