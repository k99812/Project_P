// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "PPGASCharacterPlayer.generated.h"

DECLARE_DELEGATE(FInputReleasedDelegate);

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
	//Build.cs ���� EnhancedInput ��� �߰�
	//#include "InputActionValue.h" ��� �߰�
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
	TObjectPtr<class UInputAction> ComboSprintAction;

	//�Է� �̺�Ʈ �ݹ��Լ�
	//�̶� FInputActionValue�� �Ű� ������ ����
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MoveInputReleased();
	void Sprint(const FInputActionValue& Value);
	void ComboSprint(const FInputActionValue& Value);
	void ComboSprintReleased();
};
