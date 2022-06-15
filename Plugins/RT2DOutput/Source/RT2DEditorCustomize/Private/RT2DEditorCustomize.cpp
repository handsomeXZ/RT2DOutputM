#include "RT2DEditorCustomize.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "AssetTypeAction_RT2D.h"
#include "Capture3DTool.h"
#include "3DToolCustomization.h"




IMPLEMENT_GAME_MODULE(FEditorCustomizationModule, RT2DEditorCustomize);

#define LOCTEXT_NAMESPACE "RT2DEditorCustomize"

void FEditorCustomizationModule::StartupModule() {

	RegisterAssetTools();
	RegisterCustomDetials();
}

void FEditorCustomizationModule::ShutdownModule() {

}

void FEditorCustomizationModule::RegisterAssetTools() {

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type MyAssetType = EAssetTypeCategories::Textures;
	TSharedPtr<FAssetTypeActions_TextureRenderTarget2DR32f> Action = MakeShareable(new FAssetTypeActions_TextureRenderTarget2DR32f(MyAssetType));
	AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());

}

void FEditorCustomizationModule::RegisterCustomDetials() {
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(ACapture3DTool::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&F3DToolCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FEditorCustomizationModule::UnRegisterCustomDetials() {
	if (FModuleManager::Get().IsModuleLoaded("CustomDetials"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(ACapture3DTool::StaticClass()->GetFName());
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

void FEditorCustomizationModule::UnRegisterAssetTools() {
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		EAssetTypeCategories::Type MyAssetType = EAssetTypeCategories::Textures;
		TSharedPtr<FAssetTypeActions_TextureRenderTarget2DR32f> Action = MakeShareable(new FAssetTypeActions_TextureRenderTarget2DR32f(MyAssetType));
		AssetTools.UnregisterAssetTypeActions (Action.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE  