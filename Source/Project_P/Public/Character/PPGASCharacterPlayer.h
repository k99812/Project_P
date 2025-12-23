// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameplayEffectTypes.h"
#include "Interface/PPAnimInterface.h"
#include "Interface/PPPlayerCharacterInterface.h"
#include "PPGASCharacterPlayer.generated.h"

DECLARE_DELEGATE(FInputReleasedDelegate);

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGASCharacterPlayer : public APPCharacterBase, public IAbilitySystemInterface, public IPPAnimInterface, public IPPPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	APPGASCharacterPlayer();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// InputRelease Delegate
public:
	FInputReleasedDelegate InputReleasedDelegate;

	virtual void BindInputReleasedDelegate(class UPPAnimInstance* InAnimInstance) override;

// GAS Section
protected:
	virtual void SetupGASPlayerInputComponent();

	void InitGAS();

	void InitializeAttributes();

	void GASInputPressed(int32 InputID);

	void GASInputReleased(int32 InputID);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultStatsEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilites;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<EInputAbility, TSubclassOf<class UGameplayAbility>> StartInputAbilites;

// Dead Section
protected:
	UFUNCTION()
	virtual void ActorIsDead();

public:
	virtual void SetDead() override;

	virtual void SetAlive() override;

// Ability System Section
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> ASC;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	//Build.cs 에서 EnhancedInput 모듈 추가
	//#include "InputActionValue.h" 헤더 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> LeftAttackAction;

	//입력 이벤트 콜백함수
	//이때 FInputActionValue를 매개 변수로 받음
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MoveInputReleased();

//Gameplay Tag Event
protected:
	void RemoveTag(const FGameplayTagContainer& RemoveTagContainer);

	//#include "GameplayEffectTypes.h" 추가
	virtual void OnCCTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

//Montage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> LevelStartMontage;

// Monster HPBar
public:
	virtual void RequestSetMonsterHpBar(bool bIsFound, AActor* TargetMonster) override;

protected:
	UFUNCTION(Client, Unreliable)
	void ClientRPC_SetMonsterHpBar(bool bIsFound, AActor* TargetMonster);
};
