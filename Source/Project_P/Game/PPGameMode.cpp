// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PPGameMode.h"
#include "Player/PPGASPlayerState.h"
#include "Player/PPPlayerController.h"

APPGameMode::APPGameMode()
{
	//���Ӹ�忡�� ĳ����, ĳ������Ʈ�ѷ� Ŭ���� ����
	//��������� include �ؼ� APPCharacterPlayerStaticClass(); �����ʰ� ConstructorHelpers�� �����ͷ� ���� Ŭ������ �޾� �� �� ����
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Project_P/Blueprint/Player/BPPP_CharacterPlayer.BPPP_CharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Project_P/Blueprint/BP_PPPlayerController.BP_PPPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	
	/*
	* APlayerStateŬ������ ConstructorHelpers�� �ҷ����� ���� �߻�
	* static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateClassRef(TEXT("/Script/CoreUObject.Class'/Script/Project_P.PPGASPlayerState'"));
	* if (PlayerStateClassRef.Class)
	* {
	* 	PlayerStateClass = PlayerStateClassRef.Class;
	* }
	*/
	PlayerStateClass = APPGASPlayerState::StaticClass();
}

void APPGameMode::OnPlayerDead()
{
	/* ��Ƽ�÷��� ���� �÷��̾� ��Ʈ�ѷ� ��������
	for (FConstPlayerControllerIterator itr = GetWorld()->GetPlayerControllerIterator(); itr; itr++)
	{
		APPPlayerController* PlayerController = Cast<APPPlayerController>(itr->Get());
		if(PlayerController)
		{
			PlayerController->GameOver();
		}
	}
	*/

	//�̱��÷��̴� first�÷��̾� ��Ʈ�ѷ��� �������� ��
	APPPlayerController* PlayerController = Cast<APPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("APPGameMode::OnPlayerDead Call PlayerController->GameOver"));
		PlayerController->GameOver();
	}
}