// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef PROJECT_MainChar_generated_h
#error "MainChar.generated.h already included, missing '#pragma once' in MainChar.h"
#endif
#define PROJECT_MainChar_generated_h

#define FID_Project_Source_Project_Public_MainChar_h_12_SPARSE_DATA
#define FID_Project_Source_Project_Public_MainChar_h_12_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execStartJump); \
	DECLARE_FUNCTION(execMoveRight); \
	DECLARE_FUNCTION(execMoveForward);


#define FID_Project_Source_Project_Public_MainChar_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execStartJump); \
	DECLARE_FUNCTION(execMoveRight); \
	DECLARE_FUNCTION(execMoveForward);


#define FID_Project_Source_Project_Public_MainChar_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMainChar(); \
	friend struct Z_Construct_UClass_AMainChar_Statics; \
public: \
	DECLARE_CLASS(AMainChar, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Project"), NO_API) \
	DECLARE_SERIALIZER(AMainChar)


#define FID_Project_Source_Project_Public_MainChar_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAMainChar(); \
	friend struct Z_Construct_UClass_AMainChar_Statics; \
public: \
	DECLARE_CLASS(AMainChar, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Project"), NO_API) \
	DECLARE_SERIALIZER(AMainChar)


#define FID_Project_Source_Project_Public_MainChar_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AMainChar(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMainChar) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMainChar); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMainChar); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMainChar(AMainChar&&); \
	NO_API AMainChar(const AMainChar&); \
public:


#define FID_Project_Source_Project_Public_MainChar_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AMainChar(AMainChar&&); \
	NO_API AMainChar(const AMainChar&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AMainChar); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMainChar); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AMainChar)


#define FID_Project_Source_Project_Public_MainChar_h_9_PROLOG
#define FID_Project_Source_Project_Public_MainChar_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Project_Source_Project_Public_MainChar_h_12_SPARSE_DATA \
	FID_Project_Source_Project_Public_MainChar_h_12_RPC_WRAPPERS \
	FID_Project_Source_Project_Public_MainChar_h_12_INCLASS \
	FID_Project_Source_Project_Public_MainChar_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Project_Source_Project_Public_MainChar_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Project_Source_Project_Public_MainChar_h_12_SPARSE_DATA \
	FID_Project_Source_Project_Public_MainChar_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Project_Source_Project_Public_MainChar_h_12_INCLASS_NO_PURE_DECLS \
	FID_Project_Source_Project_Public_MainChar_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> PROJECT_API UClass* StaticClass<class AMainChar>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Project_Source_Project_Public_MainChar_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
