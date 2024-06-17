// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeProject_P_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Project_P;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Project_P()
	{
		if (!Z_Registration_Info_UPackage__Script_Project_P.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Project_P",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xB7754868,
				0xEE07531F,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Project_P.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Project_P.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Project_P(Z_Construct_UPackage__Script_Project_P, TEXT("/Script/Project_P"), Z_Registration_Info_UPackage__Script_Project_P, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xB7754868, 0xEE07531F));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
