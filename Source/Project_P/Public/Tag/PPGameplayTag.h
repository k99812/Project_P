
#pragma once

#include "GameplayTagContainer.h"

#define PPTAG_ABILITY_JUMP FGameplayTag::RequestGameplayTag(FName("Ability.Jump"))
#define PPTAG_ABILITY_ATTACK FGameplayTag::RequestGameplayTag(FName("Ability.Attack"))
#define PPTAG_ABILITY_SPRINT FGameplayTag::RequestGameplayTag(FName("Ability.Sprint"))
#define PPTAG_ABILITY_HIT FGameplayTag::RequestGameplayTag(FName("Ability.Hit"))

#define PPTAG_EVENT_INPUTOPEN FGameplayTag::RequestGameplayTag(FName("Event.Input.Open"))
#define PPTAG_EVENT_INPUTRECEIVE FGameplayTag::RequestGameplayTag(FName("Event.Input.Receive"))

#define PPTAG_CHARACTER_EVENT_INPUTRECEVIE FGameplayTag::RequestGameplayTag(FName("Character.State.Event.InputReceive"))
#define PPTAG_CHARACTER_EVENT_INPUTOPEN FGameplayTag::RequestGameplayTag(FName("Character.State.Event.InputOpen"))

#define PPTAG_CHARACTER_MONSTER FGameplayTag::RequestGameplayTag(FName("Character.Monster"))
#define PPTAG_CHARACTER_PLAYER FGameplayTag::RequestGameplayTag(FName("Character.Player"))

#define PPTAG_CHARACTER_ATTACKHITCHECK FGameplayTag::RequestGameplayTag(FName("Character.Action.AttackHitCheck"))
#define PPTAG_CHARACTER_COMBOATTACK FGameplayTag::RequestGameplayTag(FName("Character.Action.ComboAttack"))

#define PPTAG_CHARACTER_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAttacking"))
#define PPTAG_CHARACTER_ISJUMPING FGameplayTag::RequestGameplayTag(FName("Character.State.IsJumping"))
#define PPTAG_CHARACTER_ISSPRINT FGameplayTag::RequestGameplayTag(FName("Character.State.IsSprint"))
#define PPTAG_CHARACTER_ISWALKING FGameplayTag::RequestGameplayTag(FName("Character.State.IsWalking"))
#define PPTAG_CHARACTER_ISCC FGameplayTag::RequestGameplayTag(FName("Character.State.IsCC"))
#define PPTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))