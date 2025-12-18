// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define PPGAS_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("%s %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_NETMODEINFO (GetNetMode() == ENetMode::NM_Client ? *FString::Printf(TEXT("CLIENT:%d"), GPlayInEditorID) : GetNetMode() == ENetMode::NM_Standalone ? TEXT("STANDALONE") : TEXT("SERVER"))
#define PPNET_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s: %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_SUBLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwningActorFromActorInfo()->GetLocalRole()))
#define LOG_SUBREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwningActorFromActorInfo()->GetRemoteRole()))
#define LOG_SUBNETMODEINFO (GetOwningActorFromActorInfo()->GetNetMode() == ENetMode::NM_Client ? *FString::Printf(TEXT("CLIENT:%d"), GPlayInEditorID) : GetOwningActorFromActorInfo()->GetNetMode() == ENetMode::NM_Standalone ? TEXT("STANDALONE") : TEXT("SERVER"))
#define PPNET_SUBLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s: %s"), LOG_SUBNETMODEINFO, LOG_SUBLOCALROLEINFO, LOG_SUBREMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_ANIMLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), MeshComp->GetOwner()->GetLocalRole()))
#define LOG_ANIMREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), MeshComp->GetOwner()->GetRemoteRole()))
#define LOG_ANIMMODEINFO (MeshComp->GetOwner()->GetNetMode() == ENetMode::NM_Client ? *FString::Printf(TEXT("CLIENT:%d"), GPlayInEditorID) : MeshComp->GetOwner()->GetNetMode() == ENetMode::NM_Standalone ? TEXT("STANDALONE") : TEXT("SERVER"))
#define PPNET_ANIMLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s: %s"), LOG_ANIMMODEINFO, LOG_ANIMLOCALROLEINFO, LOG_ANIMREMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_ATLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwnerActor()->GetLocalRole()))
#define LOG_ATREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwnerActor()->GetRemoteRole()))
#define LOG_ATMODEINFO (GetOwnerActor()->GetNetMode() == ENetMode::NM_Client ? *FString::Printf(TEXT("CLIENT:%d"), GPlayInEditorID) : GetOwnerActor()->GetNetMode() == ENetMode::NM_Standalone ? TEXT("STANDALONE") : TEXT("SERVER"))
#define PPNET_ATLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s: %s"), LOG_ATMODEINFO, LOG_ATLOCALROLEINFO, LOG_ATREMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LOG_ATTLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwningActor()->GetLocalRole()))
#define LOG_ATTREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwningActor()->GetRemoteRole()))
#define LOG_ATTMODEINFO (GetOwningActor()->GetNetMode() == ENetMode::NM_Client ? *FString::Printf(TEXT("CLIENT:%d"), GPlayInEditorID) : GetOwningActor()->GetNetMode() == ENetMode::NM_Standalone ? TEXT("STANDALONE") : TEXT("SERVER"))
#define PPNET_ATTLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s: %s"), LOG_ATTMODEINFO, LOG_ATTLOCALROLEINFO, LOG_ATTREMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogGAS, Log, All);