// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Project/Public/Testing_GameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTesting_GameModeBase() {}
// Cross Module References
	PROJECT_API UClass* Z_Construct_UClass_ATesting_GameModeBase_NoRegister();
	PROJECT_API UClass* Z_Construct_UClass_ATesting_GameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Project();
// End Cross Module References
	void ATesting_GameModeBase::StaticRegisterNativesATesting_GameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATesting_GameModeBase);
	UClass* Z_Construct_UClass_ATesting_GameModeBase_NoRegister()
	{
		return ATesting_GameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_ATesting_GameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATesting_GameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Project,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATesting_GameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Testing_GameModeBase.h" },
		{ "ModuleRelativePath", "Public/Testing_GameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATesting_GameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATesting_GameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATesting_GameModeBase_Statics::ClassParams = {
		&ATesting_GameModeBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_ATesting_GameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATesting_GameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATesting_GameModeBase()
	{
		if (!Z_Registration_Info_UClass_ATesting_GameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATesting_GameModeBase.OuterSingleton, Z_Construct_UClass_ATesting_GameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATesting_GameModeBase.OuterSingleton;
	}
	template<> PROJECT_API UClass* StaticClass<ATesting_GameModeBase>()
	{
		return ATesting_GameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATesting_GameModeBase);
	struct Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_GameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_GameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATesting_GameModeBase, ATesting_GameModeBase::StaticClass, TEXT("ATesting_GameModeBase"), &Z_Registration_Info_UClass_ATesting_GameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATesting_GameModeBase), 3286336337U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_GameModeBase_h_1025538951(TEXT("/Script/Project"),
		Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_GameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Testing_GameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
