#pragma once

#include "Engine/Texture.h"

struct Data
{
public:
	Data() :Width(0), Height(0), Distance(0) {}
	TArray<FLinearColor> DepthData;
	int32 GetWidth() {
		return Width;
	}
	int32 GetHeight() {
		return Height;
	}
	float GetDistance() {
		return Distance;
	}

	void SetWidth(int32 v) {
		Width = v;
	}
	void SetHeight(int32 v) {
		Height = v;
	}
	void SetDistance(float v) {
		Distance = v;
	}

private:
	int32 Width;
	int32 Height;
	float Distance;
};

enum ECode {
	EC_DepthStencil = 1,
	EC_RG = 2,
	EC_LDR = 3,
};
 
class R32fTool
{
public:
	static void EnterRender(TWeakObjectPtr<UTextureRenderTarget2D> inObject , UWorld* world, Data& data);
	static void EnterRender(TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Back_inObject, UWorld* world, Data& D_data);
	static void EnterRender(TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Back_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Left_inObject, 
		TWeakObjectPtr<UTextureRenderTarget2D> D_Right_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Top_inObject, 
		TWeakObjectPtr<UTextureRenderTarget2D> D_Bottom_inObject, UWorld* world, Data& D_data, Data& D_data_Left, Data& D_data_Top);
	static void TextureWriting_RenderThread(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> inObject );/* ·ÏÆú */
	static void ReadDepth(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> inObject , Data& data);
	static void ReadDepth(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject,
		TWeakObjectPtr<UTextureRenderTarget2D> D_Back_Object, Data& D_data);
	static void ReadDepth(FRHICommandListImmediate& RHICmdList, TWeakObjectPtr<UTextureRenderTarget2D> D_Front_inObject,
		TWeakObjectPtr<UTextureRenderTarget2D> D_Back_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Left_inObject,
		TWeakObjectPtr<UTextureRenderTarget2D> D_Right_inObject, TWeakObjectPtr<UTextureRenderTarget2D> D_Top_inObject,
		TWeakObjectPtr<UTextureRenderTarget2D> D_Bottom_inObject, Data& D_data, Data& D_data_Left, Data& D_data_Top);

	static void OutputFile(FString fileDestination, Data data);
	static void CreateNewStatic(FString Path, FString Name, ECode ec, Data data_R);
	static void Create_DepthStencil(FString Path, FString Name, Data data);
	static void Create_RG(FString Path, FString Name, Data data_R);
	static void Create_LDR(FString Path, FString Name, Data data);
	
};

