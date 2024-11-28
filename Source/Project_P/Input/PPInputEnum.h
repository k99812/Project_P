
#pragma once

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