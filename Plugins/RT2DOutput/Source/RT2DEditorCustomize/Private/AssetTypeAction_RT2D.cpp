// Copyright HandsomeCheese

#include "AssetTypeAction_RT2D.h"
#include "ToolMenuSection.h"
#include "EditorStyleSet.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "R32fTool.h"



#define LOCTEXT_NAMESPACE "AssetTypeAction_RT2D"

void FAssetTypeActions_TextureRenderTarget2DR32f::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	
	FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);

}



void FAssetTypeActions_TextureRenderTarget2DR32f::GetActions(const TArray<UObject *>& InObjects, FMenuBuilder& MenuBuilder) {
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);
	
	auto RenderTargets = GetTypedWeakObjectPtrs<UTextureRenderTarget2DR32f>(InObjects);

	//TArray<TWeakObjectPtr<UTextureRenderTarget2DR32f>> RenderTargets = rt;

// 	UTextureRenderTarget2D* NewRT = UKismetRenderingLibrary::CreateRenderTarget2D(InObjects[0],512,512, ETextureRenderTargetFormat::RTF_RGBA8,FLinearColor(0.0f,0.0f,0.0f),true);



	fileDestination = TEXT("C:/Users/10663/Desktop/test.png");
	MenuBuilder.AddMenuEntry(
		LOCTEXT("PlaySequenceFrame", "OutPut Texture"),
		LOCTEXT("PlaySequenceFrameToolTip", "OutPut a static texture from the selected render targets."),
		/*FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Texture2D"),*/
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([this,RenderTargets] 
				{
					Execute(RenderTargets,EMethod::EM_Output);
				}),
			FCanExecuteAction()
		)

	);
	MenuBuilder.AddMenuEntry(
		LOCTEXT("PlaySequenceFrame", "Create Texture"),
		LOCTEXT("PlaySequenceFrameToolTip", "Create a static texture from the selected render targets."),
		/*FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.Texture2D"),*/
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([this, RenderTargets]
				{
					Execute(RenderTargets, EMethod::EM_Static);
				}),
			FCanExecuteAction()
					)

	);
}


void FAssetTypeActions_TextureRenderTarget2DR32f::Execute(TArray<TWeakObjectPtr<UTextureRenderTarget2DR32f>> Objects, EMethod emthod)
{
	UE_LOG(LogTemp, Warning, TEXT("Objects: %d"), Objects.Num());
	UTextureRenderTarget2D* inObject = Objects[0].Get();
	UWorld* world = inObject->GetWorld();
	Data data;
	FString path= fileDestination;
	data.SetDistance(200.0f);
/*inObject->GameThread_GetRenderTargetResource();*/
// 	if (RTResource->GetTexture2DRHI()/* == ? RTResource->GetRenderTargetTexture()->GetTexture2D() */)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("GetTextureBaseRHI: true"));
// 	}
// 	else {
// 		UE_LOG(LogTemp, Warning, TEXT("GetTextureBaseRHI: false"));
// 	}
	
	R32fTool::EnterRender(&(inObject[0]),world, data);

	FString name = inObject[0].GetFName().ToString();
	FString gamepath = FPaths::GetBaseFilename(inObject[0].GetPathName(), false);
	gamepath.RemoveFromEnd(TEXT("/") + name);

	switch (emthod)
	{
	case EM_Output: R32fTool::OutputFile(path, data);
		break;
	case EM_Static: 
		UE_LOG(LogTemp, Warning, TEXT("path: %s name: %s"), *gamepath, *(name + TEXT("_Texture")));
		R32fTool::CreateNewStatic(gamepath, name + TEXT("_Texture"),ECode::EC_DepthStencil,data);
		break;
	case EM_Matetial:
		break;
	default: UE_LOG(LogTemp, Warning, TEXT("Error EMthod"));
		break;
	}

	return;
}




#undef LOCTEXT_NAMESPACE