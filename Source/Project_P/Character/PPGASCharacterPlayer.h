// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "PPGASCharacterPlayer.generated.h"

DECLARE_DELEGATE(FInputReleasedDelegate);

UENUM(BlueprintType)
enum class EInputAbility : uint8
{
	None UMETA(DisplayName = "None"),
	Jump = 10 UMETA(DisplayName = "Jump"),
	Sprint UMETA(DisplayName = "Sprint"),
	LeftAttack UMETA(DisplayName = "LAttack"),
	RightAttack UMETA(DisplayName = "RAttack"),
	Skill UMETA(DisplayName = "Skill")
};

/**
 * 
 */
UCLASS()
class PROJECT_P_API APPGASCharacterPlayer : public APPCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APPGASCharacterPlayer();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// GAS Section
protected:
	virtual void SetupGASPlayerInputComponent();

	void GASInputPressed(int32 InputID);

	void GASInputReleased(int32 InputID);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilites;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TMap<EInputAbility, TSubclassOf<class UGameplayAbility>> StartInputAbilites;

// InputRelease Delegate
public:
	FInputReleasedDelegate InputReleasedDelegate;

// Ability System Section
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	//Build.cs 에서 EnhancedInput 모듈 추가
	//#include "InputActionValue.h" 헤더 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPriaveteAccess = "true"))
	TObjectPtr<class UInputAction> LeftAttackAction;

	//입력 이벤트 콜백함수
	//이때 FInputActionValue를 매개 변수로 받음
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MoveInputReleased();
};
