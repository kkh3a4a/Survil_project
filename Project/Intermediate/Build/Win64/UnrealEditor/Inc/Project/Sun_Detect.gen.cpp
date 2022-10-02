// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Project/Public/Sun_Detect.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSun_Detect() {}
// Cross Module References
	PROJECT_API UClass* Z_Construct_UClass_ASun_Detect_NoRegister();
	PROJECT_API UClass* Z_Construct_UClass_ASun_Detect();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_Project();
// End Cross Module References
	void ASun_Detect::StaticRegisterNativesASun_Detect()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ASun_Detect);
	UClass* Z_Construct_UClass_ASun_Detect_NoRegister()
	{
		return ASun_Detect::StaticClass();
	}
	struct Z_Construct_UClass_ASun_Detect_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASun_Detect_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Project,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASun_Detect_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Sun_Detect.h" },
		{ "ModuleRelativePath", "Public/Sun_Detect.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASun_Detect_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASun_Detect>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ASun_Detect_Statics::ClassParams = {
		&ASun_Detect::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ASun_Detect_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ASun_Detect_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASun_Detect()
	{
		if (!Z_Registration_Info_UClass_ASun_Detect.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ASun_Detect.OuterSingleton, Z_Construct_UClass_ASun_Detect_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ASun_Detect.OuterSingleton;
	}
	template<> PROJECT_API UClass* StaticClass<ASun_Detect>()
	{
		return ASun_Detect::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASun_Detect);
	struct Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Sun_Detect_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Sun_Detect_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ASun_Detect, ASun_Detect::StaticClass, TEXT("ASun_Detect"), &Z_Registration_Info_UClass_ASun_Detect, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ASun_Detect), 2331045924U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Sun_Detect_h_3884043799(TEXT("/Script/Project"),
		Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Sun_Detect_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Project_Source_Project_Public_Sun_Detect_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
