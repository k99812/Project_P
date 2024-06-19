// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PPGameMode.h"

APPGameMode::APPGameMode()
{
	//게임모드에서 캐릭터, 캐릭터컨트롤러 클래스 지정
	//헤더파일을 include 해서 APPCharacterPlayerStaticClass(); 하지않고 ConstructorHelpers로 에디터로 부터 클래스를 받아 올 수 있음
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Blueprint/BPPP_CharacterPlayer.BPPP_CharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/Project_P.PPPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	
}
