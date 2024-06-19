// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PPGameMode.h"

APPGameMode::APPGameMode()
{
	//���Ӹ�忡�� ĳ����, ĳ������Ʈ�ѷ� Ŭ���� ����
	//��������� include �ؼ� APPCharacterPlayerStaticClass(); �����ʰ� ConstructorHelpers�� �����ͷ� ���� Ŭ������ �޾� �� �� ����
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
