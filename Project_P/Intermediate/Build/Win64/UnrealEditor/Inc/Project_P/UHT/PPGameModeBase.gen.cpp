// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Game/PPGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePPGameModeBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	PROJECT_P_API UClass* Z_Construct_UClass_APPGameModeBase();
	PROJECT_P_API UClass* Z_Construct_UClass_APPGameModeBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_Project_P();
// End Cross Module References
	void APPGameModeBase::StaticRegisterNativesAPPGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(APPGameModeBase);
	UClass* Z_Construct_UClass_APPGameModeBase_NoRegister()
	{
		return APPGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_APPGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_APPGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Project_P,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_APPGameModeBase_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APPGameModeBase_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Game/PPGameModeBase.h" },
		{ "ModuleRelativePath", "Game/PPGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_APPGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APPGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_APPGameModeBase_Statics::ClassParams = {
		&APPGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_APPGameModeBase_Statics::Class_MetaDataParams), Z_Construct_UClass_APPGameModeBase_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_APPGameModeBase()
	{
		if (!Z_Registration_Info_UClass_APPGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_APPGameModeBase.OuterSingleton, Z_Construct_UClass_APPGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_APPGameModeBase.OuterSingleton;
	}
	template<> PROJECT_P_API UClass* StaticClass<APPGameModeBase>()
	{
		return APPGameModeBase::StaticClass();
	}
	APPGameModeBase::APPGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(APPGameModeBase);
	APPGameModeBase::~APPGameModeBase() {}
	struct Z_CompiledInDeferFile_FID_Project_P_Project_P_Source_Project_P_Game_PPGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_P_Project_P_Source_Project_P_Game_PPGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_APPGameModeBase, APPGameModeBase::StaticClass, TEXT("APPGameModeBase"), &Z_Registration_Info_UClass_APPGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(APPGameModeBase), 17537246U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_P_Project_P_Source_Project_P_Game_PPGameModeBase_h_2649556146(TEXT("/Script/Project_P"),
		Z_CompiledInDeferFile_FID_Project_P_Project_P_Source_Project_P_Game_PPGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Project_P_Project_P_Source_Project_P_Game_PPGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
