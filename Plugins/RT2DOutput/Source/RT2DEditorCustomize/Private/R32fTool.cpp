#include "R32fTool.h"
#include <Textures/SlateIcon.h>
#include <Framework/Commands/UIAction.h>
#include "Engine/Texture.h"
#include "Toolkits/IToolkit.h"
#include <ImageUtils.h>
#include <Misc/FileHelper.h>
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "RHIResources.h"
#include "RHI.h"
#include "RenderingThread.h"
#include "TextureResource.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ShaderParameterUtils.h"
#include "Kismet/KismetRenderingLibrary.h"

void R32fTool::ReadDepth(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> inObject, Data& data)
{

	FTexture2DRHIRef GameBuffer = inObject->GetRenderTargetResource()->GetRenderTargetTexture();
	//FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();
	FIntRect Rect(0, 0, GameBuffer->GetSizeX(), GameBuffer->GetSizeY());

	UE_LOG(LogTemp, Warning, TEXT("SizeX: %d , SizeY: %d , Nummips: %d , NumSamples: %d"),
		GameBuffer->GetSizeX(),
		GameBuffer->GetSizeY(),
		GameBuffer->GetNumMips(),
		GameBuffer->GetNumSamples());


	const TCHAR* InDebugName = TEXT("");
	FRHIResourceCreateInfo CreateInfo(InDebugName);
	FTexture2DRHIRef CopiedTexture = RHICreateTexture2D(
		GameBuffer->GetSizeX(),
		GameBuffer->GetSizeY(),
		GameBuffer->GetFormat(),
		GameBuffer->GetNumMips(),
		GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		CreateInfo
	);


	UE_LOG(LogTemp, Warning, TEXT("StartCopyTexture"));
	RHICmdList.CopyTexture(GameBuffer, CopiedTexture, FRHICopyTextureInfo{});
	UE_LOG(LogTemp, Warning, TEXT("EndCopyTexture"));
	void* RawData = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("GameBuffer: %d"), GameBuffer->GetFormat());
	UE_LOG(LogTemp, Warning, TEXT("CopiedTexture: %d"), CopiedTexture->GetFormat());

	int32 width, height;
	RHICmdList.MapStagingSurface(CopiedTexture, RawData, width, height);
	data.SetWidth(width);
	data.SetHeight(height);

	struct Float32Color {
		FFloat32 R;
		// 		FFloat32 G;
		// 		FFloat32 B;
		// 		FFloat32 A;
	};

	TArray<Float32Color> ImgData;

	ImgData.AddUninitialized(data.GetWidth() * data.GetHeight());
	data.DepthData.AddUninitialized(data.GetWidth() * data.GetHeight());
	memcpy(ImgData.GetData(), RawData, width * height * sizeof(Float32Color));

	float dis = data.GetDistance();
	for (int i = 0; i < ImgData.Num(); i++)
	{
		data.DepthData[i] = FLinearColor(
			FMath::Clamp(1 - (ImgData[i].R.FloatValue / dis ), 0.0f, 1.0f),
			0.0f,
			0.0f
			// 			FMath::Clamp(1 - (ImgData[i].G.FloatValue / distance), 0.0f, 1.0f),
			// 			FMath::Clamp(1 - (ImgData[i].B.FloatValue / distance), 0.0f, 1.0f)
		);
	}


}
void R32fTool::ReadDepth(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject,
	TWeakObjectPtr<UTextureRenderTarget2D> D_Back_Object, Data& D_data)
{

	FTexture2DRHIRef D_Front_GameBuffer = D_Front_inObject->GetRenderTargetResource()->GetRenderTargetTexture();
	FTexture2DRHIRef D_Back_GameBuffer = D_Back_Object->GetRenderTargetResource()->GetRenderTargetTexture();

	//FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();

	UE_LOG(LogTemp, Warning, TEXT("SizeX: %d , SizeY: %d , Nummips: %d , NumSamples: %d"),
		D_Front_GameBuffer->GetSizeX(),
		D_Front_GameBuffer->GetSizeY(),
		D_Front_GameBuffer->GetNumMips(),
		D_Front_GameBuffer->GetNumSamples());

	const TCHAR* InDebugName = TEXT("");
	FRHIResourceCreateInfo  D_Front_CreateInfo(InDebugName);
	FRHIResourceCreateInfo D_Back_CreateInfo(InDebugName);



	FTexture2DRHIRef D_Front_CopiedTexture = RHICreateTexture2D(
		D_Front_GameBuffer->GetSizeX(),
		D_Front_GameBuffer->GetSizeY(),
		D_Front_GameBuffer->GetFormat(),
		D_Front_GameBuffer->GetNumMips(),
		D_Front_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Front_CreateInfo
	);
	FTexture2DRHIRef D_Back_CopiedTexture = RHICreateTexture2D(
		D_Back_GameBuffer->GetSizeX(),
		D_Back_GameBuffer->GetSizeY(),
		D_Back_GameBuffer->GetFormat(),
		D_Back_GameBuffer->GetNumMips(),
		D_Back_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Back_CreateInfo
	);

	UE_LOG(LogTemp, Warning, TEXT("StartCopyTexture"));
	RHICmdList.CopyTexture(D_Front_GameBuffer, D_Front_CopiedTexture, FRHICopyTextureInfo{});
	RHICmdList.CopyTexture(D_Back_GameBuffer, D_Back_CopiedTexture, FRHICopyTextureInfo{});

	UE_LOG(LogTemp, Warning, TEXT("EndCopyTexture"));
	UE_LOG(LogTemp, Warning, TEXT("D_Front_GameBuffer: %d"), D_Front_GameBuffer->GetFormat());
	UE_LOG(LogTemp, Warning, TEXT("D_Front_CopiedTexture: %d"), D_Front_CopiedTexture->GetFormat());
	UE_LOG(LogTemp, Warning, TEXT("D_Back_GameBuffer: %d"), D_Back_GameBuffer->GetFormat());
	UE_LOG(LogTemp, Warning, TEXT("D_Back_CopiedTexture: %d"), D_Back_CopiedTexture->GetFormat());



	int32 D_width_R, D_height_R,D_width_G,D_height_G;


	void* D_RawData_R = nullptr;
	void* D_RawData_G = nullptr;
	RHICmdList.MapStagingSurface(D_Front_CopiedTexture, D_RawData_R, D_width_R, D_height_R);
	RHICmdList.MapStagingSurface(D_Back_CopiedTexture, D_RawData_G, D_width_G, D_height_G);
	if (D_width_G!=D_width_R || D_height_G!=D_height_R)
	{
		UE_LOG(LogTemp, Warning, TEXT("Size diff"));
		return;
	}
	D_data.SetWidth(D_width_R);
	D_data.SetHeight(D_height_R);


	struct Float32Color {
		FFloat32 R;
/*		FFloat32 G;*/
	};

	TArray<Float32Color> D_Front_ImgData;
	TArray<Float32Color> D_Back_ImgData;

	D_Front_ImgData.AddUninitialized(D_data.GetWidth() * D_data.GetHeight());
	D_data.DepthData.AddUninitialized(D_data.GetWidth() * D_data.GetHeight());
	memcpy(D_Front_ImgData.GetData(), D_RawData_R, D_width_R * D_height_R * sizeof(Float32Color));
	D_Back_ImgData.AddUninitialized(D_data.GetWidth()* D_data.GetHeight());
	D_data.DepthData.AddUninitialized(D_data.GetWidth()* D_data.GetHeight());
	memcpy(D_Back_ImgData.GetData(), D_RawData_G, D_width_G* D_height_G * sizeof(Float32Color));


	if (D_Front_ImgData.Num() != D_Back_ImgData.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ImgData error"));
		return;
	}

	float D_distance = D_data.GetDistance();

	for (int i = 0; i < D_Front_ImgData.Num(); i++)
	{
		D_data.DepthData[i] = FLinearColor(
			FMath::Clamp(1 - (D_Front_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			FMath::Clamp(1 - (D_Back_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			0.0f
			// 			FMath::Clamp(1 - (ImgData[i].G.FloatValue / distance), 0.0f, 1.0f),
			// 			FMath::Clamp(1 - (ImgData[i].B.FloatValue / distance), 0.0f, 1.0f)
		);
	}

}
void R32fTool::ReadDepth(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject,
	TWeakObjectPtr<UTextureRenderTarget2D> D_Back_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Left_inObject,
	TWeakObjectPtr<UTextureRenderTarget2D> D_Right_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Top_inObject,
	TWeakObjectPtr<UTextureRenderTarget2D> D_Bottom_inObject, Data& D_data, Data& D_data_Left, Data& D_data_Top)
{
	FTexture2DRHIRef D_Front_GameBuffer = D_Front_inObject->GetRenderTargetResource()->GetRenderTargetTexture();
	FTexture2DRHIRef D_Back_GameBuffer = D_Back_inObject->GetRenderTargetResource()->GetRenderTargetTexture();

	FTexture2DRHIRef D_Left_GameBuffer = D_Left_inObject->GetRenderTargetResource()->GetRenderTargetTexture();
	FTexture2DRHIRef D_Right_GameBuffer = D_Right_inObject->GetRenderTargetResource()->GetRenderTargetTexture();
	FTexture2DRHIRef D_Top_GameBuffer = D_Top_inObject->GetRenderTargetResource()->GetRenderTargetTexture();
	FTexture2DRHIRef D_Bottom_GameBuffer = D_Bottom_inObject->GetRenderTargetResource()->GetRenderTargetTexture();

	//FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();

	UE_LOG(LogTemp, Warning, TEXT("SizeX: %d , SizeY: %d , Nummips: %d , NumSamples: %d"),
		D_Front_GameBuffer->GetSizeX(),
		D_Front_GameBuffer->GetSizeY(),
		D_Front_GameBuffer->GetNumMips(),
		D_Front_GameBuffer->GetNumSamples());

	const TCHAR* InDebugName = TEXT("");
	FRHIResourceCreateInfo  D_Front_CreateInfo(InDebugName);
	FRHIResourceCreateInfo D_Back_CreateInfo(InDebugName);
	FRHIResourceCreateInfo  D_Left_CreateInfo(InDebugName);
	FRHIResourceCreateInfo D_Right_CreateInfo(InDebugName);
	FRHIResourceCreateInfo  D_Top_CreateInfo(InDebugName);
	FRHIResourceCreateInfo D_Bottom_CreateInfo(InDebugName);

	FTexture2DRHIRef D_Front_CopiedTexture = RHICreateTexture2D(
		D_Front_GameBuffer->GetSizeX(),
		D_Front_GameBuffer->GetSizeY(),
		D_Front_GameBuffer->GetFormat(),
		D_Front_GameBuffer->GetNumMips(),
		D_Front_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Front_CreateInfo
	);
	FTexture2DRHIRef D_Back_CopiedTexture = RHICreateTexture2D(
		D_Back_GameBuffer->GetSizeX(),
		D_Back_GameBuffer->GetSizeY(),
		D_Back_GameBuffer->GetFormat(),
		D_Back_GameBuffer->GetNumMips(),
		D_Back_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Back_CreateInfo
	);


	FTexture2DRHIRef D_Left_CopiedTexture = RHICreateTexture2D(
		D_Left_GameBuffer->GetSizeX(),
		D_Left_GameBuffer->GetSizeY(),
		D_Left_GameBuffer->GetFormat(),
		D_Left_GameBuffer->GetNumMips(),
		D_Left_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Left_CreateInfo
	);
	FTexture2DRHIRef D_Right_CopiedTexture = RHICreateTexture2D(
		D_Right_GameBuffer->GetSizeX(),
		D_Right_GameBuffer->GetSizeY(),
		D_Right_GameBuffer->GetFormat(),
		D_Right_GameBuffer->GetNumMips(),
		D_Right_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Right_CreateInfo
	);
	FTexture2DRHIRef D_Top_CopiedTexture = RHICreateTexture2D(
		D_Top_GameBuffer->GetSizeX(),
		D_Top_GameBuffer->GetSizeY(),
		D_Top_GameBuffer->GetFormat(),
		D_Top_GameBuffer->GetNumMips(),
		D_Top_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Top_CreateInfo
	);
	FTexture2DRHIRef D_Bottom_CopiedTexture = RHICreateTexture2D(
		D_Bottom_GameBuffer->GetSizeX(),
		D_Bottom_GameBuffer->GetSizeY(),
		D_Bottom_GameBuffer->GetFormat(),
		D_Bottom_GameBuffer->GetNumMips(),
		D_Bottom_GameBuffer->GetNumSamples(),
		ETextureCreateFlags::CPUReadback,
		D_Bottom_CreateInfo
	);

	UE_LOG(LogTemp, Warning, TEXT("StartCopyTexture"));
	RHICmdList.CopyTexture(D_Front_GameBuffer, D_Front_CopiedTexture, FRHICopyTextureInfo{});
	RHICmdList.CopyTexture(D_Back_GameBuffer, D_Back_CopiedTexture, FRHICopyTextureInfo{});
	RHICmdList.CopyTexture(D_Left_GameBuffer, D_Left_CopiedTexture, FRHICopyTextureInfo{});
	RHICmdList.CopyTexture(D_Right_GameBuffer, D_Right_CopiedTexture, FRHICopyTextureInfo{});
	RHICmdList.CopyTexture(D_Top_GameBuffer, D_Top_CopiedTexture, FRHICopyTextureInfo{});
	RHICmdList.CopyTexture(D_Bottom_GameBuffer, D_Bottom_CopiedTexture, FRHICopyTextureInfo{});

	UE_LOG(LogTemp, Warning, TEXT("EndCopyTexture"));
	UE_LOG(LogTemp, Warning, TEXT("D_Front_GameBuffer: %d"), D_Front_GameBuffer->GetFormat());
	UE_LOG(LogTemp, Warning, TEXT("D_Front_CopiedTexture: %d"), D_Front_CopiedTexture->GetFormat());
	UE_LOG(LogTemp, Warning, TEXT("D_Back_GameBuffer: %d"), D_Back_GameBuffer->GetFormat());
	UE_LOG(LogTemp, Warning, TEXT("D_Back_CopiedTexture: %d"), D_Back_CopiedTexture->GetFormat());



	int32 D_width_R, D_height_R, D_width_G, D_height_G,
		D_width_Left, D_height_Left, D_width_Right, D_height_Right, D_width_Top, D_height_Top, D_width_Bottom, D_height_Bottom;
		

	void* D_RawData_R = nullptr;
	void* D_RawData_G = nullptr;

	void* D_RawData_Left = nullptr;
	void* D_RawData_Right = nullptr;
	void* D_RawData_Top = nullptr;
	void* D_RawData_Bottom = nullptr;
	RHICmdList.MapStagingSurface(D_Front_CopiedTexture, D_RawData_R, D_width_R, D_height_R);
	RHICmdList.MapStagingSurface(D_Back_CopiedTexture, D_RawData_G, D_width_G, D_height_G);

	RHICmdList.MapStagingSurface(D_Left_CopiedTexture, D_RawData_Left, D_width_Left, D_height_Left);
	RHICmdList.MapStagingSurface(D_Right_CopiedTexture, D_RawData_Right, D_width_Right, D_height_Right);

	RHICmdList.MapStagingSurface(D_Top_CopiedTexture, D_RawData_Top, D_width_Top, D_height_Top);
	RHICmdList.MapStagingSurface(D_Bottom_CopiedTexture, D_RawData_Bottom, D_width_Bottom, D_height_Bottom);

	if (D_width_G != D_width_R || D_height_G != D_height_R)
	{
		UE_LOG(LogTemp, Warning, TEXT("Size error"));
		return;
	}
	D_data.SetWidth(D_width_R);
	D_data.SetHeight(D_height_R);

	if (D_width_Left != D_width_Right || D_height_Left != D_height_Right)
	{
		UE_LOG(LogTemp, Warning, TEXT("Size error"));
		return;
	}
	D_data_Left.SetWidth(D_width_Left);
	D_data_Left.SetHeight(D_height_Left);

	if (D_width_Top != D_width_Bottom || D_height_Top != D_height_Bottom)
	{
		UE_LOG(LogTemp, Warning, TEXT("Size error"));
		return;
	}
	D_data_Top.SetWidth(D_width_Top);
	D_data_Top.SetHeight(D_height_Top);

	struct Float32Color {
		FFloat32 R;
		/*		FFloat32 G;*/
	};

	TArray<Float32Color> D_Front_ImgData;
	TArray<Float32Color> D_Back_ImgData;

	TArray<Float32Color> D_Left_ImgData;
	TArray<Float32Color> D_Right_ImgData;
	TArray<Float32Color> D_Top_ImgData;
	TArray<Float32Color> D_Bottom_ImgData;


	D_Front_ImgData.AddUninitialized(D_data.GetWidth()* D_data.GetHeight());
	D_data.DepthData.AddUninitialized(D_data.GetWidth()* D_data.GetHeight());
	memcpy(D_Front_ImgData.GetData(), D_RawData_R, D_width_R* D_height_R * sizeof(Float32Color));
	D_Back_ImgData.AddUninitialized(D_data.GetWidth()* D_data.GetHeight());
	D_data.DepthData.AddUninitialized(D_data.GetWidth()* D_data.GetHeight());
	memcpy(D_Back_ImgData.GetData(), D_RawData_G, D_width_G* D_height_G * sizeof(Float32Color));


	D_Left_ImgData.AddUninitialized(D_data_Left.GetWidth() * D_data_Left.GetHeight());
	D_data_Left.DepthData.AddUninitialized(D_data_Left.GetWidth() * D_data_Left.GetHeight());
	memcpy(D_Left_ImgData.GetData(), D_RawData_Left, D_width_Left * D_height_Left * sizeof(Float32Color));
	D_Right_ImgData.AddUninitialized(D_data_Left.GetWidth() * D_data_Left.GetHeight());
	D_data_Left.DepthData.AddUninitialized(D_data_Left.GetWidth() * D_data_Left.GetHeight());
	memcpy(D_Right_ImgData.GetData(), D_RawData_Right, D_width_Right * D_height_Right * sizeof(Float32Color));


	D_Top_ImgData.AddUninitialized(D_data_Top.GetWidth()* D_data_Top.GetHeight());
	D_data_Top.DepthData.AddUninitialized(D_data_Top.GetWidth()* D_data_Top.GetHeight());
	memcpy(D_Top_ImgData.GetData(), D_RawData_Top, D_width_Top* D_height_Top * sizeof(Float32Color));
	D_Bottom_ImgData.AddUninitialized(D_data_Top.GetWidth()* D_data_Top.GetHeight());
	D_data_Top.DepthData.AddUninitialized(D_data_Top.GetWidth()* D_data_Top.GetHeight());
	memcpy(D_Bottom_ImgData.GetData(), D_RawData_Bottom, D_width_Bottom* D_height_Bottom * sizeof(Float32Color));



	if (D_Front_ImgData.Num() != D_Back_ImgData.Num() || D_Left_ImgData.Num()!=D_Right_ImgData.Num() || D_Top_ImgData.Num()!=D_Bottom_ImgData.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("ImgData error"));
		return;
	}

	float D_distance = D_data.GetDistance();

	for (int i = 0; i < D_Front_ImgData.Num(); i++)
	{
		D_data.DepthData[i] = FLinearColor(
			FMath::Clamp(1 - (D_Front_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			FMath::Clamp(1 - (D_Back_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			0.0f
			// 			FMath::Clamp(1 - (ImgData[i].G.FloatValue / distance), 0.0f, 1.0f),
			// 			FMath::Clamp(1 - (ImgData[i].B.FloatValue / distance), 0.0f, 1.0f)
		);
	}

	D_distance = D_data_Left.GetDistance();

	for (int i = 0; i < D_Front_ImgData.Num(); i++)
	{
		D_data_Left.DepthData[i] = FLinearColor(
			FMath::Clamp(1 - (D_Left_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			FMath::Clamp(1 - (D_Right_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			0.0f
		);
	}

	D_distance = D_data_Top.GetDistance();

	for (int i = 0; i < D_Front_ImgData.Num(); i++)
	{
		D_data_Top.DepthData[i] = FLinearColor(
			FMath::Clamp(1 - (D_Top_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			FMath::Clamp(1 - (D_Bottom_ImgData[i].R.FloatValue / D_distance), 0.0f, 1.0f),
			0.0f
		);
	}

}
void R32fTool::EnterRender(TWeakObjectPtr<UTextureRenderTarget2D> inObject, UWorld* world, Data& data) {
	UE_LOG(LogTemp, Warning, TEXT("InGame: %s"), (IsInGameThread() ? TEXT("true") : TEXT("false")));

	check(IsInGameThread());

	FlushRenderingCommands();
	/*	ERHIFeatureLevel::Type FeatureLevel = world->Scene->GetFeatureLevel();*/
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[&inObject, &data](FRHICommandListImmediate& RHICmdList)
		{
			ReadDepth
			(
				RHICmdList,
				inObject,
				data
			);
		}
	);
	FlushRenderingCommands();
}
void R32fTool::EnterRender(TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Back_inObject, UWorld* world, Data& D_data) {
	UE_LOG(LogTemp, Warning, TEXT("InGame: %s"), (IsInGameThread() ? TEXT("true") : TEXT("false")));

	check(IsInGameThread());



	FlushRenderingCommands();
	/*	ERHIFeatureLevel::Type FeatureLevel = world->Scene->GetFeatureLevel();*/
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[&D_Front_inObject, &D_Back_inObject, &D_data](FRHICommandListImmediate& RHICmdList)
		{
			ReadDepth
			(
				RHICmdList,
				D_Front_inObject,
				D_Back_inObject,
				D_data
			);
		}
	);
	FlushRenderingCommands();
}
void R32fTool::EnterRender(TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Back_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Left_inObject,
	TWeakObjectPtr<UTextureRenderTarget2D> D_Right_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Top_inObject,
	TWeakObjectPtr<UTextureRenderTarget2D> D_Bottom_inObject, UWorld* world, Data& D_data, Data& D_data_Left, Data& D_data_Top) {
	UE_LOG(LogTemp, Warning, TEXT("InGame: %s"), (IsInGameThread() ? TEXT("true") : TEXT("false")));

	check(IsInGameThread());



	FlushRenderingCommands();
	/*	ERHIFeatureLevel::Type FeatureLevel = world->Scene->GetFeatureLevel();*/
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[&D_Front_inObject, &D_Back_inObject, &D_Left_inObject, &D_Right_inObject, &D_Top_inObject, &D_Bottom_inObject, 
		&D_data, &D_data_Left, &D_data_Top](FRHICommandListImmediate& RHICmdList)
		{
			ReadDepth
			(
				RHICmdList,
				D_Front_inObject,
				D_Back_inObject,
				D_Left_inObject,
				D_Right_inObject,
				D_Top_inObject,
				D_Bottom_inObject,
				D_data,
				D_data_Left,
				D_data_Top
			);
		}
	);
	FlushRenderingCommands();
}
/* 废弃 */
void R32fTool::TextureWriting_RenderThread(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> inObject) {
	check(IsInRenderingThread());

	// 	TShaderMapRef<FMyComputeShader> ComputeShader(GetGlobalShaderMap(FeatureLevel));
	// 	RHICmdList.SetComputeShader(ComputeShader->GetComputeShader());

	FTexture2DRHIRef TexRef2D = inObject->GetRenderTargetResource()->GetRenderTargetTexture();


	//ComputeShader->SetSurfaces(RHICmdList,)
	int32 SizeX = TexRef2D->GetSizeX();
	int32 SizeY = TexRef2D->GetSizeY();
	//	FRHIResourceCreateInfo CreateInfo;
	// 	FTexture2DRHIRef Texture = RHICreateTexture2D(SizeX, SizeY, PF_A32B32G32R32F, 1, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	// 	FUnorderedAccessViewRHIRef TextureUAV = RHICreateUnorderedAccessView(Texture);
	// 	ComputeShader->SetSurfaces(RHICmdList, TextureUAV, ShaderStructData);
	// 	DispatchComputeShader(RHICmdList, *ComputeShader, SizeX / 32, SizeY / 32, 1);
	// 	ComputeShader->UnbindBuffers(RHICmdList);
	// 
	// 	DrawTestShaderRenderTarget_RenderThread(RHICmdList, TexRef2D, FeatureLevel, FLinearColor(), Texture, ShaderStructData);


	TArray<FColor> Bitmap;
	//TArray<uint32> sourcedata;
	//-----------------------------------
	uint32 LolStride;
	char* TextureDataPtr;
	/*
		char* TextureDataPtr = (char*)RHICmdList.LockTexture2D(TexRef2D, 0, EResourceLockMode::RLM_ReadOnly, LolStride, false);
		LockTexture2D 似乎并不适用RT
	*/
	RHICmdList.LockTexture2D(TexRef2D, 0, EResourceLockMode::RLM_ReadOnly, LolStride, false);

	return;
	for (uint32 Row = 0; Row < TexRef2D->GetSizeY(); ++Row)
	{
		uint32* PixelPtr = (uint32*)TextureDataPtr;
		for (uint32 Col = 0; Col < TexRef2D->GetSizeX(); ++Col)
		{
			uint32 EncodedPixel = *PixelPtr;
			UE_LOG(LogConsoleResponse, Error, TEXT("PixelPtr: %d"), EncodedPixel);
			PixelPtr++;
		}
		TextureDataPtr += LolStride;
	}
	RHICmdList.UnlockTexture2D(TexRef2D, 0, false);
	return;
	//FUpdateTextureRegion2D region = FUpdateTextureRegion2D(0, 0, 0, 0, TexRef2D->GetSizeX(), TexRef2D->GetSizeY());
	//RHIUpdateTexture2D(TexRef2D, 0, region, sizeof(uint32) * TexRef2D->GetSizeX() * TexRef2D->GetSizeY(), (uint8*)sourcedata.GetData());

	//-----------------------------------
	//Bitmap.Reset();
	TextureDataPtr = (char*)RHICmdList.LockTexture2D(TexRef2D, 0, EResourceLockMode::RLM_ReadOnly, LolStride, true);

	for (uint32 Row = 0; Row < TexRef2D->GetSizeY(); ++Row)
	{
		uint32* PixelPtr = (uint32*)TextureDataPtr;
		for (uint32 Col = 0; Col < TexRef2D->GetSizeX(); ++Col)
		{
			uint32 EncodedPixel = *PixelPtr;
			uint8 r = (EncodedPixel & 0x000000FF);
			uint8 g = (EncodedPixel & 0x0000FF00) >> 8;
			uint8 b = (EncodedPixel & 0x00FF0000) >> 16;
			uint8 a = (EncodedPixel & 0xFF000000) >> 24;
			FColor col = FColor(r, g, b, a);
			Bitmap.Add(FColor(b, g, r, a));
			PixelPtr++;
		}
		TextureDataPtr += LolStride;
	}
	RHICmdList.UnlockTexture2D(TexRef2D, 0, false);

	if (Bitmap.Num())
	{
		IFileManager::Get().MakeDirectory(*FPaths::ScreenShotDir(), true);
		const FString ScreenFileName(FPaths::ScreenShotDir() / TEXT("VisualizeTexture"));
		uint32 ExtendXWithMSAA = Bitmap.Num() / TexRef2D->GetSizeY();
		FFileHelper::CreateBitmap(*ScreenFileName, ExtendXWithMSAA, TexRef2D->GetSizeY(), Bitmap.GetData());
		UE_LOG(LogConsoleResponse, Display, TEXT("Content was saved to \"%s\""), *FPaths::ScreenShotDir());
	}
	else
	{
		UE_LOG(LogConsoleResponse, Error, TEXT("Failed to save BMP, format or texture type is not supported"));
	}
}

void R32fTool::OutputFile(FString path, Data data) {
	if (!FPaths::GetExtension(path).IsEmpty())
	{
		path = FPaths::GetBaseFilename(path, false);
		path += TEXT(".exr");
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);



	/*	const TArray64<uint8>& ImageData = ImageWrapper->GetCompressed((int32)EImageCompressionQuality::Uncompressed);*/
	if (ImageWrapper.IsValid())
	{
		if (ImageWrapper->SetRaw(data.DepthData.GetData(), data.DepthData.GetAllocatedSize(), data.GetWidth(), data.GetHeight(), ERGBFormat::RGBAF, 32)) {

			FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(), *path);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ImageWrapper->SetRaw：false"));
		}

	}

	ImageWrapper.Reset();
}

void R32fTool::CreateNewStatic(FString Path, FString Name, ECode ec, Data data_R) {

	switch (ec)
	{
	case EC_DepthStencil:R32fTool::Create_DepthStencil(Path, Name, data_R);
		break;
	case EC_RG:R32fTool::Create_RG(Path, Name, data_R);
		break;
	case EC_LDR:R32fTool::Create_LDR(Path, Name, data_R);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Your Ecode undefinded!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Your Ecode undefinded!"));
		break;
	}


}
void R32fTool::Create_DepthStencil(FString Path, FString Name, Data data)
{
	const int sizeX = data.GetWidth();
	const int sizeY = data.GetHeight();
	const ETextureSourceFormat format = ETextureSourceFormat::TSF_BGRA8;

	TArray<FColor> MyData;
	{
		MyData.SetNumUninitialized(sizeX * sizeY);
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++)
			{
				int index = y * sizeX + x;
				MyData[index] = data.DepthData[index].ToFColor(false);
			}
		}
	}
	const FString PackageName = Path + TEXT("/") + Name;
	UPackage* pacakge = CreatePackage(*PackageName);
	pacakge->FullyLoad();
	UTexture2D* Tex = NewObject<UTexture2D>(pacakge, FName(*Name), RF_Public | RF_Standalone);
	Tex->Source.Init(sizeX, sizeX, 1, 1, format);
	Tex->CompressionSettings = TEnumAsByte<enum TextureCompressionSettings>::EnumType::TC_Normalmap;
	uint32* BufferAddress = (uint32*)Tex->Source.LockMip(0);
	const int32 BufferSize = Tex->Source.CalcMipSize(0);
	FMemory::Memcpy(BufferAddress, MyData.GetData(), BufferSize);
	Tex->Source.UnlockMip(0);
	Tex->MarkPackageDirty();
	Tex->PostEditChange();
};
void R32fTool::Create_RG(FString Path, FString Name, Data data_R)
{
	const int sizeX = data_R.GetWidth();
	const int sizeY = data_R.GetHeight();
	const ETextureSourceFormat format = ETextureSourceFormat::TSF_BGRA8;

	TArray<FColor> MyData;
	{
		MyData.SetNumUninitialized(sizeX * sizeY);
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++)
			{
				MyData[y * sizeX + x].R = data_R.DepthData[y * sizeX + x].ToFColor(false).R;
				//MyData[y * sizeX + x].G = data_R.DepthData[y * sizeX + sizeX - x - 1].ToFColor(false).G;
				MyData[y * sizeX + x].G = data_R.DepthData[y * sizeX + x].ToFColor(false).G;
			}
		}
	}
	const FString PackageName = Path + TEXT("/") + Name;
	UPackage* pacakge = CreatePackage(*PackageName);
	pacakge->FullyLoad();
	UTexture2D* Tex = NewObject<UTexture2D>(pacakge, FName(*Name), RF_Public | RF_Standalone);
	Tex->Source.Init(sizeX, sizeX, 1, 1, format);
	Tex->CompressionSettings = TEnumAsByte<enum TextureCompressionSettings>::EnumType::TC_Normalmap;
	uint32* BufferAddress = (uint32*)Tex->Source.LockMip(0);
	const int32 BufferSize = Tex->Source.CalcMipSize(0);
	FMemory::Memcpy(BufferAddress, MyData.GetData(), BufferSize);
	Tex->Source.UnlockMip(0);
	Tex->MarkPackageDirty();
	Tex->PostEditChange();
};
void R32fTool::Create_LDR(FString Path, FString Name, Data data)
{
	const int sizeX = data.GetWidth();
	const int sizeY = data.GetHeight();
	const ETextureSourceFormat format = ETextureSourceFormat::TSF_BGRA8;

	TArray<FColor> MyData;
	{
		MyData.SetNumUninitialized(sizeX * sizeY);
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++)
			{
				int index = y * sizeX + x;
				MyData[index] = data.DepthData[index].ToFColor(false);
			}
		}
	}
	const FString PackageName = Path + TEXT("/") + Name;
	UPackage* pacakge = CreatePackage(*PackageName);
	pacakge->FullyLoad();
	UTexture2D* Tex = NewObject<UTexture2D>(pacakge, FName(*Name), RF_Public | RF_Standalone);
	
	Tex->Source.Init(sizeX, sizeY, 1, 1, format);
	Tex->CompressionSettings = TEnumAsByte<enum TextureCompressionSettings>::EnumType::TC_HDR;

	uint32* BufferAddress = (uint32*)Tex->Source.LockMip(0);
	const int32 BufferSize = Tex->Source.CalcMipSize(0);

	UE_LOG(LogTemp, Warning, TEXT("MipsizeX: %d  DataSizeX: %d"), BufferSize, sizeX);
	FMemory::Memcpy(BufferAddress, MyData.GetData(), BufferSize);
	Tex->Source.UnlockMip(0);
	Tex->MarkPackageDirty();
	Tex->PostEditChange();
};

/* 废弃 */
//void FAssetTypeActions_TextureRenderTarget2DR32f::getDepth() {
// 	struct DepthPixel	//定义深度像素结构体
// 	{
// 		float depth;
// 		char stencil;
// 		char unused1;
// 		char unused2;
// 		char unused3;
// 	};
// 	float* cpuDataPtr;	// Texture深度值数组首地址
// 	TArray<DepthPixel> mydata;	//最终获取色深度值数据
// 	FIntPoint buffsize;	//深度长宽大小X和Y
// 
// 	ENQUEUE_RENDER_COMMAND(ReadSurfaceFloatCommand)(	// 将读取深度数据的命令推给渲染线程进行执行
// 		[&cpuDataPtr, &mydata, &buffsize](FRHICommandListImmediate& RHICmdList) //&cpuDataPtr, &mydata, &buffsize为传入的外部参数
// 		{
// 			FSceneRenderTargets::Get(RHICmdList).AdjustGBufferRefCount(RHICmdList, 1);
// 			FTexture2DRHIRef uTex2DRes = FSceneRenderTargets::Get(RHICmdList).GetSceneDepthSurface();
// 			buffsize = uTex2DRes->GetSizeXY();
// 			uint32 sx = buffsize.X;
// 			uint32 sy = buffsize.Y;
// 			mydata.AddUninitialized(sx * sy);
// 			uint32 Lolstrid = 0;
// 			cpuDataPtr = (float*)RHILockTexture2D(uTex2DRes, 0, RLM_ReadOnly, Lolstrid, true);	// 加锁 获取可读depth Texture深度值数组首地址
// 			memcpy(mydata.GetData(), cpuDataPtr, sx * sy * sizeof(DepthPixel));		//复制深度数据
// 			RHIUnlockTexture2D(uTex2DRes, 0, true);	//解锁
// 			FSceneRenderTargets::Get(RHICmdList).AdjustGBufferRefCount(RHICmdList, -1);
// 
// 		});
// 	FlushRenderingCommands();	//等待渲染线程执行
//}


/*
/*废弃*/
// 	readPixelFlags.SetLinearToGamma(true);
// 
// 	UE_LOG(LogTemp, Warning, TEXT("Format: %d"), RTResource->GetRenderTargetTexture()->GetFormat());
// 	FRHIResourceInfo OutResourceInfo;
// 
// 	bool bGetResourceInfo =  RTResource->GetRenderTargetTexture()->GetResourceInfo(OutResourceInfo);
// 
// 	UE_LOG(LogTemp, Warning, TEXT("bGetResourceInfo: %s"), bGetResourceInfo? TEXT("true"):TEXT("False"));
// 	
// 
// 	RTResource->ReadLinearColorPixels(LinearColor, readPixelFlags);



/* 废弃 *///---------------------------------------------------------------------------------------------------------------
// 	const int32 ImageWidth = RTResource->GetSizeXY().X;
// 	const int32 ImageHeight = RTResource->GetSizeXY().Y;
// 	UE_LOG(LogTemp, Warning, TEXT("color%d"), outBMP.Num());
// 	UE_LOG(LogTemp, Warning, TEXT("linearcolor%d"), LinearColor.Num());
// 	float maxFloat = 0.0f;
// 	uint8 maxUint8 = 0;
// 	
// 	for (int32 X = 0; X < LinearColor.Num(); X++)
// 	{
// 	
// 		outBMP[X] = (LinearColor[X]/400).ToFColor(false);
// // 		outBMP[Y * ImageWidth + X].G = (uint8)((SourceData[X] / 400).G);
// // 		outBMP[Y * ImageWidth + X].B = (uint8)((SourceData[X] / 400).B);
// // 		outBMP[Y * ImageWidth + X].A = (uint8)((SourceData[X] / 400).A);
// 		if (LinearColor[X].R > maxFloat) {
// 			maxFloat = LinearColor[X].R;
// 			maxUint8 = (uint8)((LinearColor[X] / 400).R); 
// 		}
// 	
// 	}
// 	UE_LOG(LogTemp, Warning, TEXT("MaxFloat%d"), maxFloat);
// 	UE_LOG(LogTemp, Warning, TEXT("MaxUint8%d"), maxUint8);
// 	//---------------------------------------------------------------------------------------------------------------
// 
// 
// 	FIntPoint destSize(Objects[0]->GetSurfaceWidth(), Objects[0]->GetSurfaceHeight());
// 
// 	TArray<uint8> CompressedBitmap;
// 
// 	FImageUtils::CompressImageArray(destSize.X, destSize.Y, outBMP, CompressedBitmap);
// 
// 	const TArray64<uint8> binary = (TArray64<uint8>)CompressedBitmap;
// 
// 	bool imageSavedOk = FFileHelper::SaveArrayToFile(binary, fileDestination);
