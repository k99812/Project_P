// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PPGameOverUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


UPPGameOverUserWidget::UPPGameOverUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPPGameOverUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//BtnRestart->OnClicked.AddDynamic(this, &UPPGameOverUserWidget::BtnEventGameRestart);
}

void UPPGameOverUserWidget::BtnEventGameRestart()
{
	APlayerController* OwingPlayerController = GetOwningPlayer();

	if (OwingPlayerController)
	{
		//Ŀ�ǵ�� ���� �����
		//GetWorld()->Exec(GetWorld(), TEXT("RestartLevel"));
		//OwingPlayerController->ConsoleCommand(TEXT("RestartLevel"));

		//#include "Kismet/GameplayStatics.h" �߰�
		//OpenLevel�� �̿��� ���� �����
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Level_1")));

		//����� �Է� ��Ȱ��ȭ
		OwingPlayerController->SetShowMouseCursor(false);
		OwingPlayerController->DisableInput(OwingPlayerController);

		//�Լ������ �ش� ���� ����
		RemoveFromParent();
	}
}
