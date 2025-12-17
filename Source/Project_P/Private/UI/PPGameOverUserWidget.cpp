// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGameOverUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Interface/PPPlayerInterface.h"


UPPGameOverUserWidget::UPPGameOverUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPGameOverUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//BtnRestart->OnClicked.AddDynamic(this, &UPPGameOverUserWidget::BtnEventGameRestart);

	PlayAnimation(Color);
}

void UPPGameOverUserWidget::BtnEventGameRestart()
{
	APlayerController* OwingPlayerController = GetOwningPlayer();
	IPPPlayerInterface* Player = Cast<IPPPlayerInterface>(OwingPlayerController);
	
	if (OwingPlayerController && Player)
	{
		Player->RequestRespawn();

		OwingPlayerController->SetShowMouseCursor(false);
		OwingPlayerController->SetInputMode(FInputModeGameOnly());

		RemoveFromParent();
	}

	/*
	if (OwingPlayerController)
	{
		//커맨드로 레벨 재시작
		//GetWorld()->Exec(GetWorld(), TEXT("RestartLevel"));
		//OwingPlayerController->ConsoleCommand(TEXT("RestartLevel"));

		//#include "Kismet/GameplayStatics.h" 추가
		//OpenLevel을 이용해 레벨 재시작
		//UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Level_1")));
		UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetFName());

		//사용자 입력 비활성화
		OwingPlayerController->SetShowMouseCursor(false);
		OwingPlayerController->DisableInput(OwingPlayerController);

		//함수실행시 해당 위젯 제거
		RemoveFromParent();
	}
	*/
}