// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Dungeoneer/Public/Dungeon.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDungeon() {}
// Cross Module References
	DUNGEONEER_API UEnum* Z_Construct_UEnum_Dungeoneer_EDungeonDirection();
	UPackage* Z_Construct_UPackage__Script_Dungeoneer();
	DUNGEONEER_API UScriptStruct* Z_Construct_UScriptStruct_FDungeonPalette();
	DUNGEONEER_API UScriptStruct* Z_Construct_UScriptStruct_FDungeonModel();
	DUNGEONEER_API UScriptStruct* Z_Construct_UScriptStruct_FDungeonTile();
	DUNGEONEER_API UScriptStruct* Z_Construct_UScriptStruct_FDungeonCustomModel();
	GAMEPLAYTAGS_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayTagContainer();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
	ENGINE_API UClass* Z_Construct_UClass_UMaterial_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMesh_NoRegister();
	DUNGEONEER_API UClass* Z_Construct_UClass_ADungeon_NoRegister();
	DUNGEONEER_API UClass* Z_Construct_UClass_ADungeon();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FIntVector();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialInstanceDynamic_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UInstancedStaticMeshComponent_NoRegister();
// End Cross Module References
	static UEnum* EDungeonDirection_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_Dungeoneer_EDungeonDirection, Z_Construct_UPackage__Script_Dungeoneer(), TEXT("EDungeonDirection"));
		}
		return Singleton;
	}
	template<> DUNGEONEER_API UEnum* StaticEnum<EDungeonDirection>()
	{
		return EDungeonDirection_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EDungeonDirection(EDungeonDirection_StaticEnum, TEXT("/Script/Dungeoneer"), TEXT("EDungeonDirection"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_Dungeoneer_EDungeonDirection_Hash() { return 3101042549U; }
	UEnum* Z_Construct_UEnum_Dungeoneer_EDungeonDirection()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_Dungeoneer();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EDungeonDirection"), 0, Get_Z_Construct_UEnum_Dungeoneer_EDungeonDirection_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EDungeonDirection::NORTH", (int64)EDungeonDirection::NORTH },
				{ "EDungeonDirection::SOUTH", (int64)EDungeonDirection::SOUTH },
				{ "EDungeonDirection::EAST", (int64)EDungeonDirection::EAST },
				{ "EDungeonDirection::WEST", (int64)EDungeonDirection::WEST },
				{ "EDungeonDirection::DOWN", (int64)EDungeonDirection::DOWN },
				{ "EDungeonDirection::UP", (int64)EDungeonDirection::UP },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "DOWN.Name", "EDungeonDirection::DOWN" },
				{ "EAST.Name", "EDungeonDirection::EAST" },
				{ "ModuleRelativePath", "Public/Dungeon.h" },
				{ "NORTH.Name", "EDungeonDirection::NORTH" },
				{ "SOUTH.Name", "EDungeonDirection::SOUTH" },
				{ "UP.Name", "EDungeonDirection::UP" },
				{ "WEST.Name", "EDungeonDirection::WEST" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_Dungeoneer,
				nullptr,
				"EDungeonDirection",
				"EDungeonDirection",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FDungeonPalette::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern DUNGEONEER_API uint32 Get_Z_Construct_UScriptStruct_FDungeonPalette_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FDungeonPalette, Z_Construct_UPackage__Script_Dungeoneer(), TEXT("DungeonPalette"), sizeof(FDungeonPalette), Get_Z_Construct_UScriptStruct_FDungeonPalette_Hash());
	}
	return Singleton;
}
template<> DUNGEONEER_API UScriptStruct* StaticStruct<FDungeonPalette>()
{
	return FDungeonPalette::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FDungeonPalette(FDungeonPalette::StaticStruct, TEXT("/Script/Dungeoneer"), TEXT("DungeonPalette"), false, nullptr, nullptr);
static struct FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonPalette
{
	FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonPalette()
	{
		UScriptStruct::DeferCppStructOps<FDungeonPalette>(FName(TEXT("DungeonPalette")));
	}
} ScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonPalette;
	struct Z_Construct_UScriptStruct_FDungeonPalette_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Models_ValueProp;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_Models_Key_KeyProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Models_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_Models;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonPalette_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FDungeonPalette>();
	}
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models_ValueProp = { "Models", nullptr, (EPropertyFlags)0x0000000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UScriptStruct_FDungeonModel, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models_Key_KeyProp = { "Models_Key", nullptr, (EPropertyFlags)0x0000000000000001, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models_MetaData[] = {
		{ "Category", "DungeonPalette" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models = { "Models", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonPalette, Models), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FDungeonPalette_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonPalette_Statics::NewProp_Models,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FDungeonPalette_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_Dungeoneer,
		nullptr,
		&NewStructOps,
		"DungeonPalette",
		sizeof(FDungeonPalette),
		alignof(FDungeonPalette),
		Z_Construct_UScriptStruct_FDungeonPalette_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonPalette_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonPalette_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonPalette_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FDungeonPalette()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FDungeonPalette_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_Dungeoneer();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("DungeonPalette"), sizeof(FDungeonPalette), Get_Z_Construct_UScriptStruct_FDungeonPalette_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FDungeonPalette_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FDungeonPalette_Hash() { return 2823162568U; }
class UScriptStruct* FDungeonTile::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern DUNGEONEER_API uint32 Get_Z_Construct_UScriptStruct_FDungeonTile_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FDungeonTile, Z_Construct_UPackage__Script_Dungeoneer(), TEXT("DungeonTile"), sizeof(FDungeonTile), Get_Z_Construct_UScriptStruct_FDungeonTile_Hash());
	}
	return Singleton;
}
template<> DUNGEONEER_API UScriptStruct* StaticStruct<FDungeonTile>()
{
	return FDungeonTile::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FDungeonTile(FDungeonTile::StaticStruct, TEXT("/Script/Dungeoneer"), TEXT("DungeonTile"), false, nullptr, nullptr);
static struct FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonTile
{
	FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonTile()
	{
		UScriptStruct::DeferCppStructOps<FDungeonTile>(FName(TEXT("DungeonTile")));
	}
} ScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonTile;
	struct Z_Construct_UScriptStruct_FDungeonTile_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_SegmentModels_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SegmentModels_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_SegmentModels;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_CustomModels_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CustomModels_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_CustomModels;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Tags_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Tags;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonTile_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FDungeonTile_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FDungeonTile>();
	}
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_SegmentModels_Inner = { "SegmentModels", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_SegmentModels_MetaData[] = {
		{ "Category", "DungeonTile" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_SegmentModels = { "SegmentModels", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonTile, SegmentModels), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_SegmentModels_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_SegmentModels_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_CustomModels_Inner = { "CustomModels", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FDungeonCustomModel, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_CustomModels_MetaData[] = {
		{ "Category", "DungeonTile" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_CustomModels = { "CustomModels", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonTile, CustomModels), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_CustomModels_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_CustomModels_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_Tags_MetaData[] = {
		{ "Category", "DungeonTile" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_Tags = { "Tags", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonTile, Tags), Z_Construct_UScriptStruct_FGameplayTagContainer, METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_Tags_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_Tags_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FDungeonTile_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_SegmentModels_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_SegmentModels,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_CustomModels_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_CustomModels,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonTile_Statics::NewProp_Tags,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FDungeonTile_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_Dungeoneer,
		nullptr,
		&NewStructOps,
		"DungeonTile",
		sizeof(FDungeonTile),
		alignof(FDungeonTile),
		Z_Construct_UScriptStruct_FDungeonTile_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonTile_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonTile_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonTile_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FDungeonTile()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FDungeonTile_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_Dungeoneer();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("DungeonTile"), sizeof(FDungeonTile), Get_Z_Construct_UScriptStruct_FDungeonTile_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FDungeonTile_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FDungeonTile_Hash() { return 3652410656U; }
class UScriptStruct* FDungeonCustomModel::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern DUNGEONEER_API uint32 Get_Z_Construct_UScriptStruct_FDungeonCustomModel_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FDungeonCustomModel, Z_Construct_UPackage__Script_Dungeoneer(), TEXT("DungeonCustomModel"), sizeof(FDungeonCustomModel), Get_Z_Construct_UScriptStruct_FDungeonCustomModel_Hash());
	}
	return Singleton;
}
template<> DUNGEONEER_API UScriptStruct* StaticStruct<FDungeonCustomModel>()
{
	return FDungeonCustomModel::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FDungeonCustomModel(FDungeonCustomModel::StaticStruct, TEXT("/Script/Dungeoneer"), TEXT("DungeonCustomModel"), false, nullptr, nullptr);
static struct FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonCustomModel
{
	FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonCustomModel()
	{
		UScriptStruct::DeferCppStructOps<FDungeonCustomModel>(FName(TEXT("DungeonCustomModel")));
	}
} ScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonCustomModel;
	struct Z_Construct_UScriptStruct_FDungeonCustomModel_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TemplateName_MetaData[];
#endif
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_TemplateName;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Offset_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Offset;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FDungeonCustomModel>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_TemplateName_MetaData[] = {
		{ "Category", "DungeonCustomModel" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_TemplateName = { "TemplateName", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonCustomModel, TemplateName), METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_TemplateName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_TemplateName_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_Offset_MetaData[] = {
		{ "Category", "DungeonCustomModel" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_Offset = { "Offset", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonCustomModel, Offset), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_Offset_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_Offset_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_TemplateName,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::NewProp_Offset,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_Dungeoneer,
		nullptr,
		&NewStructOps,
		"DungeonCustomModel",
		sizeof(FDungeonCustomModel),
		alignof(FDungeonCustomModel),
		Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FDungeonCustomModel()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FDungeonCustomModel_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_Dungeoneer();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("DungeonCustomModel"), sizeof(FDungeonCustomModel), Get_Z_Construct_UScriptStruct_FDungeonCustomModel_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FDungeonCustomModel_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FDungeonCustomModel_Hash() { return 457564110U; }
class UScriptStruct* FDungeonModel::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern DUNGEONEER_API uint32 Get_Z_Construct_UScriptStruct_FDungeonModel_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FDungeonModel, Z_Construct_UPackage__Script_Dungeoneer(), TEXT("DungeonModel"), sizeof(FDungeonModel), Get_Z_Construct_UScriptStruct_FDungeonModel_Hash());
	}
	return Singleton;
}
template<> DUNGEONEER_API UScriptStruct* StaticStruct<FDungeonModel>()
{
	return FDungeonModel::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FDungeonModel(FDungeonModel::StaticStruct, TEXT("/Script/Dungeoneer"), TEXT("DungeonModel"), false, nullptr, nullptr);
static struct FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonModel
{
	FScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonModel()
	{
		UScriptStruct::DeferCppStructOps<FDungeonModel>(FName(TEXT("DungeonModel")));
	}
} ScriptStruct_Dungeoneer_StaticRegisterNativesFDungeonModel;
	struct Z_Construct_UScriptStruct_FDungeonModel_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Materials_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Materials_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Materials;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Mesh_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Mesh;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonModel_Statics::Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FDungeonModel_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FDungeonModel>();
	}
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Materials_Inner = { "Materials", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UMaterial_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Materials_MetaData[] = {
		{ "Category", "DungeonModel" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Materials = { "Materials", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonModel, Materials), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Materials_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Materials_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Mesh_MetaData[] = {
		{ "Category", "DungeonModel" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Mesh = { "Mesh", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FDungeonModel, Mesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Mesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Mesh_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FDungeonModel_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Materials_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Materials,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FDungeonModel_Statics::NewProp_Mesh,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FDungeonModel_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_Dungeoneer,
		nullptr,
		&NewStructOps,
		"DungeonModel",
		sizeof(FDungeonModel),
		alignof(FDungeonModel),
		Z_Construct_UScriptStruct_FDungeonModel_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonModel_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FDungeonModel_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FDungeonModel_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FDungeonModel()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FDungeonModel_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_Dungeoneer();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("DungeonModel"), sizeof(FDungeonModel), Get_Z_Construct_UScriptStruct_FDungeonModel_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FDungeonModel_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FDungeonModel_Hash() { return 515122357U; }
	void ADungeon::StaticRegisterNativesADungeon()
	{
	}
	UClass* Z_Construct_UClass_ADungeon_NoRegister()
	{
		return ADungeon::StaticClass();
	}
	struct Z_Construct_UClass_ADungeon_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Tiles_ValueProp;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Tiles_Key_KeyProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Tiles_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_Tiles;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Scale;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SelectionMaterial_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SelectionMaterial;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PlusIconMaterial_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_PlusIconMaterial;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TileSelectedMaterial_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TileSelectedMaterial;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TileUnselectedMaterial_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TileUnselectedMaterial;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TileHoveredUnselectedMaterial_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TileHoveredUnselectedMaterial;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TileHoveredSelectedMaterial_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TileHoveredSelectedMaterial;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DungeonPalette_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_DungeonPalette;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SelectedTemplate_MetaData[];
#endif
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_SelectedTemplate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DungeonQuad_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_DungeonQuad;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ISMCs_ValueProp;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_ISMCs_Key_KeyProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ISMCs_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_ISMCs;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_ISMCValues_ValueProp;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ISMCValues_Key_KeyProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ISMCValues_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_ISMCValues;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ADungeon_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_Dungeoneer,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Dungeon.h" },
		{ "IsBlueprintBase", "false" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles_ValueProp = { "Tiles", nullptr, (EPropertyFlags)0x0000000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UScriptStruct_FDungeonTile, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles_Key_KeyProp = { "Tiles_Key", nullptr, (EPropertyFlags)0x0000000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FIntVector, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles_MetaData[] = {
		{ "Category", "Dungeon" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles = { "Tiles", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, Tiles), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_Scale_MetaData[] = {
		{ "Category", "Dungeon" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, Scale), METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_Scale_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_Scale_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_SelectionMaterial_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_SelectionMaterial = { "SelectionMaterial", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, SelectionMaterial), Z_Construct_UClass_UMaterial_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_SelectionMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_SelectionMaterial_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_PlusIconMaterial_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_PlusIconMaterial = { "PlusIconMaterial", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, PlusIconMaterial), Z_Construct_UClass_UMaterial_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_PlusIconMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_PlusIconMaterial_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_TileSelectedMaterial_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_TileSelectedMaterial = { "TileSelectedMaterial", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, TileSelectedMaterial), Z_Construct_UClass_UMaterialInstanceDynamic_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_TileSelectedMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_TileSelectedMaterial_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_TileUnselectedMaterial_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_TileUnselectedMaterial = { "TileUnselectedMaterial", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, TileUnselectedMaterial), Z_Construct_UClass_UMaterialInstanceDynamic_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_TileUnselectedMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_TileUnselectedMaterial_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredUnselectedMaterial_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredUnselectedMaterial = { "TileHoveredUnselectedMaterial", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, TileHoveredUnselectedMaterial), Z_Construct_UClass_UMaterialInstanceDynamic_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredUnselectedMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredUnselectedMaterial_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredSelectedMaterial_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredSelectedMaterial = { "TileHoveredSelectedMaterial", nullptr, (EPropertyFlags)0x0010000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, TileHoveredSelectedMaterial), Z_Construct_UClass_UMaterialInstanceDynamic_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredSelectedMaterial_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredSelectedMaterial_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonPalette_MetaData[] = {
		{ "Category", "Dungeon" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonPalette = { "DungeonPalette", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, DungeonPalette), Z_Construct_UScriptStruct_FDungeonPalette, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonPalette_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonPalette_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_SelectedTemplate_MetaData[] = {
		{ "Category", "Dungeon" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_SelectedTemplate = { "SelectedTemplate", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, SelectedTemplate), METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_SelectedTemplate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_SelectedTemplate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonQuad_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonQuad = { "DungeonQuad", nullptr, (EPropertyFlags)0x0040000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, DungeonQuad), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonQuad_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonQuad_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs_ValueProp = { "ISMCs", nullptr, (EPropertyFlags)0x0000000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UClass_UInstancedStaticMeshComponent_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs_Key_KeyProp = { "ISMCs_Key", nullptr, (EPropertyFlags)0x0000000000080008, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs = { "ISMCs", nullptr, (EPropertyFlags)0x0040008000000008, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, ISMCs), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs_MetaData)) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues_ValueProp = { "ISMCValues", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues_Key_KeyProp = { "ISMCValues_Key", nullptr, (EPropertyFlags)0x0000000000080000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UInstancedStaticMeshComponent_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues_MetaData[] = {
		{ "ModuleRelativePath", "Public/Dungeon.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues = { "ISMCValues", nullptr, (EPropertyFlags)0x0040008000000000, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADungeon, ISMCValues), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ADungeon_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_Tiles,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_Scale,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_SelectionMaterial,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_PlusIconMaterial,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_TileSelectedMaterial,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_TileUnselectedMaterial,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredUnselectedMaterial,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_TileHoveredSelectedMaterial,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonPalette,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_SelectedTemplate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_DungeonQuad,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCs,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADungeon_Statics::NewProp_ISMCValues,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ADungeon_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ADungeon>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ADungeon_Statics::ClassParams = {
		&ADungeon::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ADungeon_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::PropPointers),
		0,
		0x008000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ADungeon_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ADungeon_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ADungeon()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ADungeon_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ADungeon, 1971243680);
	template<> DUNGEONEER_API UClass* StaticClass<ADungeon>()
	{
		return ADungeon::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ADungeon(Z_Construct_UClass_ADungeon, &ADungeon::StaticClass, TEXT("/Script/Dungeoneer"), TEXT("ADungeon"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADungeon);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
