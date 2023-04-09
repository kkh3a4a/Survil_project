// Fill out your copyright notice in the Description page of Project Settings.


#include "MakeMaterial.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Texture.h"
#include "PackageTools.h"
#include "AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/PackageTools.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"
// Sets default values
AMakeMaterial::AMakeMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMakeMaterial::BeginPlay()
{
	Super::BeginPlay();

	

	


	//UMaterialExpressionConstant3Vector* BaseColorFactorNode = NewObject<UMaterialExpressionConstant3Vector>(NewMaterial);
	//BaseColorFactorNode->Constant = FColor::Red; //you can specify any color here;
	//material->BaseColor.Connect(0, BaseColorFactorNode);


	//Decal = GetWorld()->SpawnActor<ADecalActor>(Location, Rotation);
	//Decal->SetActorScale3D(FVector(10, 10, 10));
	//Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//Decal->SetDecalMaterial(material);







	//const std::size_t width = 240;
	//const std::size_t height = 100;

	//TArray<FColor> Pixels;
	//Pixels.Init(FColor(0, 0, 0, 255), width * height);

	//// Define the two colors for the checkerboard pattern
	//FColor Color1 = FColor(0, 255, 0, 255); // White
	//FColor Color2 = FColor(255, 0, 0, 255); // Black

	//for (int y = 0; y < height; y++)
	//{
	//	for (int x = 0; x < width; x++)
	//	{
	//		bool IsEvenX = x % 2 == 0; // Alternate between two colors every 50 pixels on the x-axis
	//		bool IsEvenY = y % 2 == 0; // Alternate between two colors every 50 pixels on the y-axis

	//		if ((IsEvenX && !IsEvenY) || (!IsEvenX && IsEvenY))
	//		{
	//			// Set the pixel color to Color1
	//			Pixels[y * width + x] = Color1;
	//		}
	//		else
	//		{
	//			// Set the pixel color to Color2
	//			Pixels[y * width + x] = Color2;
	//		}
	//	}
	//}

	//FString PackageName = TEXT("/Game/ProceduralTextures/");
	//FString TextureName = TEXT("FlowOutput");
	//PackageName += TextureName;
	//UPackage* Package = CreatePackage(NULL, *PackageName);
	//Package->FullyLoad();

	//UTexture2D* res = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	//res->AddToRoot();
	//res->SRGB = 1;
	//res->MipGenSettings = TMGS_NoMipmaps;

	//res->PlatformData = new FTexturePlatformData();	// Then we initialize the PlatformData
	//res->PlatformData->SizeX = width;
	//res->PlatformData->SizeY = height;
	//res->PlatformData->PixelFormat = EPixelFormat::PF_R8G8B8A8;
	//res->UpdateResource();

	//// Allocate first mipmap.
	//FTexture2DMipMap* Mip = new FTexture2DMipMap();
	//res->PlatformData->Mips.Add(Mip);
	//Mip->SizeX = width;
	//Mip->SizeY = height;

	//Mip->BulkData.Lock(LOCK_READ_WRITE);
	//uint8* TextureData = static_cast<uint8*>(Mip->BulkData.Realloc(width * height * sizeof(FColor)));
	//FMemory::Memcpy(TextureData, Pixels.GetData(), width * height * sizeof(FColor));
	//Mip->BulkData.Unlock();

	//// Apply Texture changes to GPU memory
	//res->UpdateResource();

	//res->Source.Init(
	//	width,
	//	height,
	//	1, 1, ETextureSourceFormat::TSF_RGBA8, reinterpret_cast<uint8*>(Pixels.GetData()));

	//res->UpdateResource();
	//Package->MarkPackageDirty();
	//FAssetRegistryModule::AssetCreated(res);

	//FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	//bool bSaved = UPackage::SavePackage(Package, res, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);
	//check(bSaved);





	////Create texture
	//const int Width = 240;
	//const int Height = 100;

	//FColor Color1 = FColor(0, 0, 255, 255);
	//FColor Color2 = FColor(0, 255, 0, 255);

	//TArray<FColor> Pixels;
	//Pixels.Init(FColor(0, 0, 0, 255), Width * Height);
	//for (int y = 0; y < Height; y++)
	//{
	//	for (int x = 0; x < Width; x++)
	//	{
	//		bool IsEvenX = x % 2 == 0; // Alternate between two colors every 50 pixels on the x-axis
	//		bool IsEvenY = y % 2 == 0; // Alternate between two colors every 50 pixels on the y-axis

	//		if ((IsEvenX && !IsEvenY) || (!IsEvenX && IsEvenY))
	//		{
	//			// Set the pixel color to Color1
	//			Pixels[y * Width + x] = Color1;
	//		}
	//		else
	//		{
	//			// Set the pixel color to Color2
	//			Pixels[y * Width + x] = Color2;
	//		}
	//	}
	//}

	//UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, EPixelFormat::PF_R8G8B8A8);
	//Texture->SRGB = false;
	//Texture->MipGenSettings = TMGS_NoMipmaps;
	//void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	//FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
	//Texture->PlatformData->Mips[0].BulkData.Unlock();
	//Texture->UpdateResource();

	////Create material
	//Material = NewObject<UMaterial>();
	//UMaterialExpressionTextureSampleParameter2D* TextureSample = NewObject<UMaterialExpressionTextureSampleParameter2D>(Material);
	//TextureSample->SamplerType = SAMPLERTYPE_LinearColor;
	//TextureSample->Texture = Texture;

	//Material->BlendMode = EBlendMode::BLEND_Translucent;
	//Material->MaterialDomain = EMaterialDomain::MD_DeferredDecal;
	//Material->Expressions.Add(TextureSample);
	//Material->BaseColor.Connect(0, TextureSample);
	//Material->PostEditChange();

	//FVector Location(0, 0, 0);
	//FRotator Rotation = FRotator(0, 0, 0);
	//FActorSpawnParameters SpawnInfo;
	//AActor* ground = GetWorld()->SpawnActor<AActor>(terrain, Location, Rotation, SpawnInfo);

	//Decal = GetWorld()->SpawnActor<ADecalActor>(Location, Rotation);
	//Decal->SetActorScale3D(FVector(10, 10, 24));
	//Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//Decal->SetDecalMaterial(Material);



	/*for (int32 i = 0; i < ExampleMaterial->Expressions.Num(); i++)
	{
		UMaterialExpression* Expression = ExampleMaterial->Expressions[i];
		UE_LOG(LogTemp, Display, TEXT("Expression %d: %s (%s)"), i, *Expression->GetName(), *Expression->GetClass()->GetName());

		UMaterialExpressionMaterialFunctionCall* FunctionCall = Cast<UMaterialExpressionMaterialFunctionCall>(ExampleMaterial->Expressions[0]);
		if (FunctionCall)
		{
			TArray<FFunctionExpressionInput>& Inputs = FunctionCall->FunctionInputs;
			for (int32 j = 0; j < Inputs.Num(); j++)
			{
				FFunctionExpressionInput& Input = Inputs[j];
				UE_LOG(LogTemp, Display, TEXT("Input %d: %s"), j, *Input.ExpressionInput.GetClass()->GetName());

				if (j == 1)
				{
					UMaterialExpression* Value = Cast<UMaterialExpression*>(FLinearColor(1.0f, 2.0f, 0.f));
				}
				else if (j == 2)
				{
					UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Input.Input.Expression);
					if (ScalarParam)
					{
						ScalarParam->DefaultValue = 10.0f;
						UE_LOG(LogTemp, Display, TEXT("2"));
					}
					
				}
				else if (j == 3)
				{
					UMaterialExpressionScalarParameter* ScalarParam = Cast<UMaterialExpressionScalarParameter>(Input.Input.Expression);
					if (ScalarParam)
					{
						ScalarParam->DefaultValue = 0.5f;
						UE_LOG(LogTemp, Display, TEXT("3"));
					}
				}
			}
		}
	}*/
	int _X = 16;
	int _Y = 16;
	float _SpaceX = 1.f / _X;
	float _SpaceY = 1.f / _Y;
	float _Radius = 0.1f;
	float _Density = 0.2f;

	FString MaterialName = TEXT("MyMaterial");
	FString PackageName = FString(TEXT("/Game/Materials/")) + MaterialName;

	// Create a material package
	UPackage* MaterialPackage = CreatePackage(NULL, *PackageName);
	MaterialPackage->AddToRoot();
	MaterialPackage->FullyLoad();

	//Create material
	Material = NewObject<UMaterial>(MaterialPackage, *PackageName, RF_Public | RF_Standalone | RF_MarkAsRootSet);
	Material->BlendMode = EBlendMode::BLEND_Translucent;
	Material->MaterialDomain = EMaterialDomain::MD_DeferredDecal;

	for (int x = 0; x < _X; x++) {
		for (int y = 0; y < _Y; y++) {
			// Create a material expression and set its properties
			UMaterialExpressionMaterialFunctionCall* FunctionCall = NewObject<UMaterialExpressionMaterialFunctionCall>(Material);
			FunctionCall->MaterialFunction = Func;
			Material->Expressions.Add(FunctionCall);
			Material->PostEditChange();

			TArray<FFunctionExpressionInput>& Inputs = FunctionCall->FunctionInputs;
			for (int32 j = 0; j < Inputs.Num(); j++)
			{
				UE_LOG(LogTemp, Display, TEXT("%d"), j);
				if (j == 1) {
					// Set Center(V2)
					UMaterialExpressionVectorParameter* Center = NewObject<UMaterialExpressionVectorParameter>(Material);
					Center->DefaultValue = FLinearColor(_SpaceX * x, _SpaceY * y, 0.f);
					Center->ParameterName = FName("Center");
					Material->Expressions.Add(Center);
					Inputs[j].Input.Connect(0, Center);
				}
				else if (j == 2) {
					// Set Radius(S)
					UMaterialExpressionScalarParameter* Radius = NewObject<UMaterialExpressionScalarParameter>(Material);
					Radius->DefaultValue = _Radius;
					Radius->ParameterName = FName("Radius");
					Material->Expressions.Add(Radius);
					Inputs[j].Input.Connect(0, Radius);
				}
				else if (j == 3) {
					// Set Density(S)
					UMaterialExpressionScalarParameter* Density = NewObject<UMaterialExpressionScalarParameter>(Material);
					Density->DefaultValue = _Density;
					Density->ParameterName = FName("Density");
					Material->Expressions.Add(Density);
					Inputs[j].Input.Connect(0, Density);
				}
			}

			UMaterialExpressionConstant4Vector* Color = NewObject<UMaterialExpressionConstant4Vector>(Material);
			Color->Constant = FLinearColor(1.f, 0.0f, 0.0f, 1.0f);

			UMaterialExpressionMultiply* MultiplyExpression = NewObject<UMaterialExpressionMultiply>(Material);
			Material->Expressions.Add(MultiplyExpression);

			MultiplyExpression->A.Connect(0, Color);
			MultiplyExpression->B.Connect(0, FunctionCall);

			Multiplies.Add(MultiplyExpression);
		}
	}
	for (int i = 1; i < Multiplies.Num(); i += 2){
		UMaterialExpressionAdd* Add = NewObject<UMaterialExpressionAdd>(Material);
		Material->Expressions.Add(Add);
		Add->A.Connect(0, Multiplies[i - 1]);
		Add->B.Connect(0, Multiplies[i]);
		Adds.Add(Add);
	}
	TArray<UMaterialExpressionAdd*> CurrAdds;
	UMaterialExpressionAdd* LastAdd{};
	do {
		for (int i = 1; i < Adds.Num(); i+=2){
			UMaterialExpressionAdd* Add = NewObject<UMaterialExpressionAdd>(Material);
			Material->Expressions.Add(Add);
			Add->A.Connect(0, Adds[i - 1]);
			Add->B.Connect(0, Adds[i]);
			CurrAdds.Add(Add);
			if (i == Adds.Num() - 1) {
				LastAdd = Add;
			}
		}
		Adds.Empty();
		Adds = CurrAdds;
		CurrAdds.Empty();

	}while (Adds.Num() > 1);
	
	if(LastAdd)
		Material->EmissiveColor.Connect(0, LastAdd);


	Material->PostEditChange();

	FVector Location(0, 0, 0);
	FRotator Rotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;
	AActor* ground = GetWorld()->SpawnActor<AActor>(terrain, Location, Rotation, SpawnInfo);

	Decal = GetWorld()->SpawnActor<ADecalActor>(Location, Rotation);
	Decal->SetActorScale3D(FVector(10, 10, 10));
	Decal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Decal->SetDecalMaterial(Material);



	Material->PostEditChange();

	
	MaterialPackage->SetDirtyFlag(true);
	MaterialPackage->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(Material);
	// Save the material package
	FString MaterialPackageName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	if (UPackage::SavePackage(MaterialPackage, Material, RF_Standalone, *MaterialPackageName))
	{
		// Print a success message
		UE_LOG(LogTemp, Warning, TEXT("Material saved to %s"), *MaterialPackageName);
	}
	else
	{
		// Print an error message
		UE_LOG(LogTemp, Error, TEXT("Failed to save material to %s"), *MaterialPackageName);
	}
}

// Called every frame
void AMakeMaterial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
