// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PPGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Player/PPPlayerController.h"

APPGameMode::APPGameMode()
{
	//게임모드에서 캐릭터, 캐릭터컨트롤러 클래스 지정
	//헤더파일을 include 해서 APPCharacterPlayerStaticClass(); 하지않고 ConstructorHelpers로 에디터로 부터 클래스를 받아 올 수 있음
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
	
	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateClassRef(TEXT("/Script/CoreUObject.Class'/Script/Project_P.PPGASPlayerState'"));
	if (PlayerStateClassRef.Class)
	{
		PlayerStateClass = PlayerStateClassRef.Class;
	}
}

void APPGameMode::OnPlayerDead()
{
	// 멀티플레이 기준 플레이어 컨트롤러 가져오기
	for (FConstPlayerControllerIterator itr = GetWorld()->GetPlayerControllerIterator(); itr; itr++)
	{
		APPPlayerController* PlayerController = Cast<APPPlayerController>(itr->Get());
		if(PlayerController && PlayerController->IsLocalPlayerController())
		{
			PlayerController->GameOver();
		}
	}
	

	//싱글플레이는 first플레이어 컨트롤러를 가져오면 됨
	/*
	APPPlayerController* PlayerController = Cast<APPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GameOver();
	}
	*/
}

void APPGameMode::OnTakeDamage(const float& Damage, const FVector& ActorPosition)
{
	APPPlayerController* PlayerController = Cast<APPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->ActorTakedDamage(Damage, ActorPosition);
	}
}
