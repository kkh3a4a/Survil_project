// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Project/Public/MainChar.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMainChar() {}
// Cross Module References
	PROJECT_API UClass* Z_Construct_UClass_AMainChar_NoRegister();
	PROJECT_API UClass* Z_Construct_UClass_AMainChar();
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	UPackage* Z_Construct_UPackage__Script_Project();
// End Cross Module References
	DEFINE_FUNCTION(AMainChar::execStartJump)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->StartJump();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(AMainChar::execMoveRight)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Value);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->MoveRight(Z_Param_Value);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(AMainChar::execMoveForward)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Value);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->MoveForward(Z_Param_Value);
		P_NATIVE_END;
	}
	void AMainChar::StaticRegisterNativesAMainChar()
	{
		UClass* Class = AMainChar::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "MoveForward", &AMainChar::execMoveForward },
			{ "MoveRight", &AMainChar::execMoveRight },
			{ "StartJump", &AMainChar::execStartJump },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AMainChar_MoveForward_Statics
	{
		struct MainChar_eventMoveForward_Parms
		{
			float Value;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Value;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_AMainChar_MoveForward_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(MainChar_eventMoveForward_Parms, Value), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AMainChar_MoveForward_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AMainChar_MoveForward_Statics::NewProp_Value,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AMainChar_MoveForward_Statics::Function_MetaDataParams[] = {
		{ "Comment", "// ???? ?\xcc\xb5?\n" },
		{ "ModuleRelativePath", "Public/MainChar.h" },
		{ "ToolTip", "???? ?\xcc\xb5?" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMainChar_MoveForward_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AMainChar, nullptr, "MoveForward", nullptr, nullptr, sizeof(Z_Construct_UFunction_AMainChar_MoveForward_Statics::MainChar_eventMoveForward_Parms), Z_Construct_UFunction_AMainChar_MoveForward_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AMainChar_MoveForward_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AMainChar_MoveForward_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AMainChar_MoveForward_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AMainChar_MoveForward()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMainChar_MoveForward_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_AMainChar_MoveRight_Statics
	{
		struct MainChar_eventMoveRight_Parms
		{
			float Value;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Value;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_AMainChar_MoveRight_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(MainChar_eventMoveRight_Parms, Value), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AMainChar_MoveRight_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AMainChar_MoveRight_Statics::NewProp_Value,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AMainChar_MoveRight_Statics::Function_MetaDataParams[] = {
		{ "Comment", "// ?\xc2\xbf? ?\xcc\xb5?\n" },
		{ "ModuleRelativePath", "Public/MainChar.h" },
		{ "ToolTip", "?\xc2\xbf? ?\xcc\xb5?" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMainChar_MoveRight_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AMainChar, nullptr, "MoveRight", nullptr, nullptr, sizeof(Z_Construct_UFunction_AMainChar_MoveRight_Statics::MainChar_eventMoveRight_Parms), Z_Construct_UFunction_AMainChar_MoveRight_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AMainChar_MoveRight_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AMainChar_MoveRight_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AMainChar_MoveRight_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AMainChar_MoveRight()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMainChar_MoveRight_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_AMainChar_StartJump_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AMainChar_StartJump_Statics::Function_MetaDataParams[] = {
		{ "Comment", "// \xc5\xb0?? ?????? ???? ?\xc3\xb7??\xd7\xb8? ?????\xd5\xb4\xcf\xb4?.\n" },
		{ "ModuleRelativePath", "Public/MainChar.h" },
		{ "ToolTip", "\xc5\xb0?? ?????? ???? ?\xc3\xb7??\xd7\xb8? ?????\xd5\xb4\xcf\xb4?." },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AMainChar_StartJump_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AMainChar, nullptr, "StartJump", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AMainChar_StartJump_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AMainChar_StartJump_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AMainChar_StartJump()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AMainChar_StartJump_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMainChar);
	UClass* Z_Construct_UClass_AMainChar_NoRegister()
	{
		return AMainChar::StaticClass();
	}
	struct Z_Construct_UClass_AMainChar_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AMainChar_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_Project,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_AMainChar_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AMainChar_MoveForward, "MoveForward" }, // 3867491127
		{ &Z_Construct_UFunction_AMainChar_MoveRight, "MoveRight" }, // 3272216795
		{ &Z_Construct_UFunction_AMainChar_StartJump, "StartJump" }, // 813660566
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AMainChar_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "MainChar.h" },
		{ "ModuleRelativePath", "Public/MainChar.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AMainChar_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMainChar>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AMainChar_Statics::ClassParams = {
		&AMainChar::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AMainChar_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AMainChar_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AMainChar()
	{
		if (!Z_Registration_Info_UClass_AMainChar.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMainChar.OuterSingleton, Z_Construct_UClass_AMainChar_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AMainChar.OuterSingleton;
	}
	template<> PROJECT_API UClass* StaticClass<AMainChar>()
	{
		return AMainChar::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AMainChar);
	struct Z_CompiledInDeferFile_FID_Project_Source_Project_Public_MainChar_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_MainChar_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AMainChar, AMainChar::StaticClass, TEXT("AMainChar"), &Z_Registration_Info_UClass_AMainChar, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMainChar), 667044915U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Project_Source_Project_Public_MainChar_h_4107402661(TEXT("/Script/Project"),
		Z_CompiledInDeferFile_FID_Project_Source_Project_Public_MainChar_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Project_Source_Project_Public_MainChar_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
