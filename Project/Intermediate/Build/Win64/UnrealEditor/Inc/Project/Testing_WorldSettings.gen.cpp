// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Project/Public/Testing_WorldSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTesting_WorldSettings() {}
// Cross Module References
	PROJECT_API UClass* Z_Construct_UClass_ATesting_WorldSettings_NoRegister();
	PROJECT_API UClass* Z_Construct_UClass_ATesting_WorldSettings();
	ENGINE_API UClass* Z_Construct_UClass_AWorldSettings();
	UPackage* Z_Construct_UPackage__Script_Project();
// End Cross Module References
	void ATesting_WorldSettings::StaticRegisterNativesATesting_WorldSettings()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATesting_WorldSettings);
	UClass* Z_Construct_UClass_ATesting_WorldSettings_NoRegister()
	{
		return ATesting_WorldSettings::StaticClass();
	}
	struct Z_Construct_UClass_ATesting_WorldSettings_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATesting_WorldSettings_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AWorldSettings,
		(UObject* (*)())Z_Construct_UPackage__Script_Project,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATesting_WorldSettings_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Actor Advanced Display Events Object Attachment Info Input Blueprint Layers Tags Replication Input Movement Collision Transformation HLOD DataLayers" },
		{ "IncludePath", "Testing_WorldSettings.h" },
		{ "ModuleRelativePath", "Public/Testing_WorldSettings.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATesting_WorldSettings_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATesting_WorldSettings>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATesting_WorldSettings_Statics::ClassParams = {
		&ATesting_WorldSettings::StaticClass,
		"game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002A4u,
		METADATA_PARAMS(Z_Construct_UClass_ATesting_WorldSettings_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATesting_WorldSettings_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATesting_WorldSettings()
	{
		if (!Z_Registration_Info_UClass_ATesting_WorldSettings.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATesting_WorldSettings.OuterSingleton, Z_Construct_UClass_ATesting_WorldSettings_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATesting_WorldSettings.OuterSingleton;
	}
	template<> PROJECT_API UClass* StaticClass<ATesting_WorldSettings>()
	{
		return ATesting_WorldSettings::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATesting_WorldSettings);
	struct Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_WorldSettings_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_WorldSettings_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATesting_WorldSettings, ATesting_WorldSettings::StaticClass, TEXT("ATesting_WorldSettings"), &Z_Registration_Info_UClass_ATesting_WorldSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATesting_WorldSettings), 1921332307U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_WorldSettings_h_3736102704(TEXT("/Script/Project"),
		Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_WorldSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_WorldSettings_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
