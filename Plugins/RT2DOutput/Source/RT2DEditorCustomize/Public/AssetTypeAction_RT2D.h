// Copyright HandsomeCheese

#pragma once

#include "CoreMinimal.h"
#include "TextureRenderTarget2DR32f.h"
#include "AssetTypeCategories.h"
#include "AssetTypeActions_Base.h"
#include "Engine/Texture.h"

enum EMethod {
	EM_Output = 1,
	EM_Static = 2,
	EM_Matetial = 3,
};

class FAssetTypeActions_TextureRenderTarget2DR32f : public FAssetTypeActions_Base
{
public:

	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InType 该资产的自定义分类, 在外面注册再传进来
	 */
	FAssetTypeActions_TextureRenderTarget2DR32f(EAssetTypeCategories::Type InType) { MyAssetType = InType; }

public:
	// IAssetTypeActions Implementation
/*	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_TextureRenderTarget2DR32f", "Render Target"); }*/

	// 资产分类, 此处使用自定义类型, 或已有类型:EAssetTypeCategories::Misc
	virtual uint32 GetCategories() override { return MyAssetType; }
	virtual FText GetName() const override { return FText::FromName(TEXT("TexturePlayerAsset")); }
	virtual UClass* GetSupportedClass() const override {  return UTextureRenderTarget2DR32f::StaticClass(); }
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	virtual FColor GetTypeColor() const override { return FColor::Purple; }
	
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }


	virtual bool CanFilter() override { return true; }
 	virtual bool IsImportedAsset() const override { return false; }


/*	virtual uint32 GetCategories() override { return EAssetTypeCategories::Textures; }*/

private:
	void Execute(TArray<TWeakObjectPtr<UTextureRenderTarget2DR32f>> Objects, EMethod emthod);

private:
	EAssetTypeCategories::Type MyAssetType;
	FString fileDestination;
};

