// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Project/ProjectGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeProjectGameModeBase() {}
// Cross Module References
	PROJECT_API UClass* Z_Construct_UClass_AProjectGameModeBase_NoRegister();
	PROJECT_API UClass* Z_Construct_UClass_AProjectGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Project();
// End Cross Module References
	void AProjectGameModeBase::StaticRegisterNativesAProjectGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AProjectGameModeBase);
	UClass* Z_Construct_UClass_AProjectGameModeBase_NoRegister()
	{
		return AProjectGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AProjectGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AProjectGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Project,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AProjectGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "ProjectGameModeBase.h" },
		{ "ModuleRelativePath", "ProjectGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AProjectGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AProjectGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AProjectGameModeBase_Statics::ClassParams = {
		&AProjectGameModeBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_AProjectGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AProjectGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AProjectGameModeBase()
	{
		if (!Z_Registration_Info_UClass_AProjectGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AProjectGameModeBase.OuterSingleton, Z_Construct_UClass_AProjectGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AProjectGameModeBase.OuterSingleton;
	}
	template<> PROJECT_API UClass* StaticClass<AProjectGameModeBase>()
	{
		return AProjectGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AProjectGameModeBase);
	struct Z_CompiledInDeferFile_FID_Project_Source_Project_ProjectGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_ProjectGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AProjectGameModeBase, AProjectGameModeBase::StaticClass, TEXT("AProjectGameModeBase"), &Z_Registration_Info_UClass_AProjectGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AProjectGameModeBase), 2802187003U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_ProjectGameModeBase_h_2798365888(TEXT("/Script/Project"),
		Z_CompiledInDeferFile_FID_Project_Source_Project_ProjectGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Project_Source_Project_ProjectGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
