// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PPDamageUISubsystem.h"
#include "UI/PPFloatingTextUserWidget.h"
#include "Project_P.h"

UPPDamageUISubsystem::UPPDamageUISubsystem()
{

}

void UPPDamageUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	PPGAS_LOG(LogGAS, Warning, TEXT("Damage UI Subsystem Created!"));
}

bool UPPDamageUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	if (IsRunningDedicatedServer())
	{
		return false;
	}

	return true;
}

void UPPDamageUISubsystem::ShowDamageUI(TSubclassOf<class UPPFloatingTextUserWidget> WidgetClass, float Damage, const FVector& WorldLocation, APlayerController* OwnerPC)
{
	if (!WidgetClass || !OwnerPC) return;

	UPPFloatingTextUserWidget* DamageWidget = nullptr;

	if (DamageUIPool.Num() > 0)
	{
		DamageWidget = DamageUIPool.Pop();
	}
	else
	{
		DamageWidget = CreateWidget<UPPFloatingTextUserWidget>(OwnerPC, WidgetClass);
	}

	if (DamageWidget)
	{
		if (!DamageWidget->IsInViewport())
		{
			DamageWidget->AddToViewport();
		}

		DamageWidget->SetTextWidget(Damage, WorldLocation);
	}
}

void UPPDamageUISubsystem::ReturnToPool(UPPFloatingTextUserWidget* Widget)
{
	if (Widget)
	{
		Widget->RemoveFromParent();
		DamageUIPool.Add(Widget);
	}
}
