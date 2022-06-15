// Fill out your copyright notice in the Description page of Project Settings.


#include "Capture3DTool.h"
#include "TextureRenderTarget2DR32f.h"
#include "SceneCaptureComponent2DR32f.h"
#include "Input/Reply.h"
#include "R32fTool.h"
#include "TimerManager.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Components/BoxComponent.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/MaterialExpressionCustom.h"
#include "Materials/MaterialExpressionTextureCoordinate.h"
#include "Materials/MaterialExpressionCameraVectorWS.h"
#include "Materials/MaterialExpressionTextureObject.h"
#include "Materials/MaterialExpressionComponentMask.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "Materials/MaterialExpressionConstant.h"
#include "Materials/MaterialExpressionDDX.h"
#include "Materials/MaterialExpressionDDY.h"
#include "Materials/MaterialExpressionConstant3Vector.h"
#include "Materials/MaterialExpressionScreenPosition.h"
#include "Materials/MaterialExpressionConstant4Vector.h"
#include "Materials/MaterialExpressionTransform.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionWorldPosition.h"
#include <Engine/EngineTypes.h>

// Sets default values
ACapture3DTool::ACapture3DTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	D_Front_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("D_Front_CaptureComponent2D"));
	D_Back_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("D_Back_CaptureComponent2D"));
	R_Front_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("R_Front_CaptureComponent2D"));
	R_Back_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("R_Back_CaptureComponent2D"));

	 

	D_Back_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
	R_Front_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
	R_Back_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	D_Front_CaptureComponent2D->SetupAttachment(RootComponent);
	D_Back_CaptureComponent2D->SetupAttachment(RootComponent);
	R_Front_CaptureComponent2D->SetupAttachment(RootComponent);
	R_Back_CaptureComponent2D->SetupAttachment(RootComponent);

	Box->SetupAttachment(RootComponent);


	Size = 512.0f;

	D_Back_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0,180)));
	R_Back_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 180)));

	D_Front_CaptureComponent2D->SetRelativeLocation(FVector(-1.0f * Size / 2.0f,0,0));
	D_Back_CaptureComponent2D->SetRelativeLocation(FVector( Size / 2.0f,0,0));

	R_Front_CaptureComponent2D->SetRelativeLocation(FVector(-1.0f * Size / 2.0f, 0, 0));
	R_Back_CaptureComponent2D->SetRelativeLocation(FVector(Size / 2.0f, 0, 0));

	Box->SetBoxExtent(FVector(1.0f, 1.0f, 1.0f));
	Box->SetRelativeScale3D(FVector(Size/2.0f, Size / 2.0f, Size / 2.0f));
	this->SetActorRelativeScale3D(FVector(1, 1, 1));

	R_Front_CaptureComponent2D->CaptureSource = TEnumAsByte<enum ESceneCaptureSource>::EnumType::SCS_FinalColorLDR;
	R_Back_CaptureComponent2D->CaptureSource = TEnumAsByte<enum ESceneCaptureSource>::EnumType::SCS_FinalColorLDR;

	/* Plus----------------------------------------------------------------------------------------*/


	D_Left_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("D_Left_CaptureComponent2D"));
	D_Right_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("D_Right_CaptureComponent2D"));
	R_Left_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("R_Left_CaptureComponent2D"));
	R_Right_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("R_Right_CaptureComponent2D"));

	D_Top_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("D_Top_CaptureComponent2D"));
	D_Bottom_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("D_Bottom_CaptureComponent2D"));
	R_Top_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("R_Top_CaptureComponent2D"));
	R_Bottom_CaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2DR32f>(TEXT("R_Bottom_CaptureComponent2D"));

// 	D_Left_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
 	D_Right_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
 	R_Left_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
 	R_Right_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));

// 	D_Top_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
 	D_Bottom_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
 	R_Top_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));
 	R_Bottom_CaptureComponent2D->SetRelativeScale3D(FVector(0, 0, 0));

	D_Left_CaptureComponent2D->SetupAttachment(RootComponent);
	D_Right_CaptureComponent2D->SetupAttachment(RootComponent);
	R_Left_CaptureComponent2D->SetupAttachment(RootComponent);
	R_Right_CaptureComponent2D->SetupAttachment(RootComponent);
	D_Top_CaptureComponent2D->SetupAttachment(RootComponent);
	D_Bottom_CaptureComponent2D->SetupAttachment(RootComponent);
	R_Top_CaptureComponent2D->SetupAttachment(RootComponent);
	R_Bottom_CaptureComponent2D->SetupAttachment(RootComponent);

	D_Left_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 90)));
	D_Right_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, -90)));
	R_Left_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, 90)));
	R_Right_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 0, -90)));


	D_Top_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, -90, 0)));
	D_Bottom_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 90, 0)));
	R_Top_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, -90, 0)));
	R_Bottom_CaptureComponent2D->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, 90, 0)));


	D_Left_CaptureComponent2D->SetRelativeLocation(FVector(0, -1.0f * Size / 2.0f, 0));
	D_Right_CaptureComponent2D->SetRelativeLocation(FVector(0, Size / 2.0f, 0));
	R_Left_CaptureComponent2D->SetRelativeLocation(FVector(0, -1.0f * Size / 2.0f, 0));
	R_Right_CaptureComponent2D->SetRelativeLocation(FVector(0, Size / 2.0f, 0));

	D_Top_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, Size / 2.0f));
	D_Bottom_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, -1.0f * Size / 2.0f));
	R_Top_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, Size / 2.0f));
	R_Bottom_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, -1.0f * Size / 2.0f));

	R_Left_CaptureComponent2D->CaptureSource = TEnumAsByte<enum ESceneCaptureSource>::EnumType::SCS_FinalColorLDR;
	R_Right_CaptureComponent2D->CaptureSource = TEnumAsByte<enum ESceneCaptureSource>::EnumType::SCS_FinalColorLDR;
	R_Top_CaptureComponent2D->CaptureSource = TEnumAsByte<enum ESceneCaptureSource>::EnumType::SCS_FinalColorLDR;
	R_Bottom_CaptureComponent2D->CaptureSource = TEnumAsByte<enum ESceneCaptureSource>::EnumType::SCS_FinalColorLDR;
	/*---------------------------------------------------------------------------------------------*/
}

// Called when the game starts or when spawned
void ACapture3DTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACapture3DTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

FReply ACapture3DTool::Refresh() {

	//强制全部重新加载
	if (!ShowOnlyActorsR.IsEmpty()) {
		D_Front_CaptureComponent2D->ShowOnlyActors.Empty();
		D_Back_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Front_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Back_CaptureComponent2D->ShowOnlyActors.Empty();
		/* Plus----------------------------------------------------------------------------------------*/
		D_Left_CaptureComponent2D->ShowOnlyActors.Empty();
		D_Right_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Left_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Right_CaptureComponent2D->ShowOnlyActors.Empty();


		D_Top_CaptureComponent2D->ShowOnlyActors.Empty();
		D_Bottom_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Top_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Bottom_CaptureComponent2D->ShowOnlyActors.Empty();

		/* --------------------------------------------------------------------------------------------*/

		LastShowOnlyActorsR.Empty();

		D_Front_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		LastShowOnlyActorsR.Append(ShowOnlyActorsR);
		D_Back_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Front_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Back_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		/* Plus----------------------------------------------------------------------------------------*/
		D_Left_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		D_Right_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Left_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Right_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);

		D_Top_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		D_Bottom_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Top_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Bottom_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		/* --------------------------------------------------------------------------------------------*/
	}


	this->SetActorRelativeScale3D(FVector(1, 1, 1));
	Box->SetRelativeScale3D(FVector(Size / 2.0f, Size / 2.0f, Size / 2.0f));

	if (D_Front_TextureTarget)
	{
		D_Front_CaptureComponent2D->TextureTarget = D_Front_TextureTarget;
	}
	if (D_Back_TextureTarget)
	{
		D_Back_CaptureComponent2D->TextureTarget = D_Back_TextureTarget;
	}
	if (R_Front_TextureTarget)
	{
		R_Front_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
		R_Front_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
		R_Front_CaptureComponent2D->TextureTarget = R_Front_TextureTarget;
	}
	if (R_Back_TextureTarget)
	{
		R_Back_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
		R_Back_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
		R_Back_CaptureComponent2D->TextureTarget = R_Back_TextureTarget;
	}
	D_Front_CaptureComponent2D->OrthoWidth = Size;
	D_Back_CaptureComponent2D->OrthoWidth = Size;
	R_Front_CaptureComponent2D->OrthoWidth = Size;
	R_Back_CaptureComponent2D->OrthoWidth = Size;

	D_Front_CaptureComponent2D->SetRelativeLocation(FVector(-1.0f * Size/2.0f, 0, 0));
	D_Back_CaptureComponent2D->SetRelativeLocation(FVector(Size / 2.0f, 0, 0));
	R_Front_CaptureComponent2D->SetRelativeLocation(FVector(-1.0f * Size / 2.0f, 0, 0));
	R_Back_CaptureComponent2D->SetRelativeLocation(FVector(Size / 2.0f, 0, 0));
// 	if (Front_CaptureComponent2D->TextureTarget) {
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Front_CaptureComponent2D->TextureTarget->GetName());
// 	}
// 	else {
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Front_TextureTarget: Empty"));
// 	}

	
	/* Plus----------------------------------------------------------------------------------------*/
	if (PrintMethod== PM_Print3D_Plus || PM_Print3D_Plus_Halton)
	{

		if (D_Left_TextureTarget)
		{
			D_Left_CaptureComponent2D->TextureTarget = D_Left_TextureTarget;
		}
		if (D_Right_TextureTarget)
		{
			D_Right_CaptureComponent2D->TextureTarget = D_Right_TextureTarget;
		}

		if (D_Top_TextureTarget)
		{
			D_Top_CaptureComponent2D->TextureTarget = D_Top_TextureTarget;
		}
		if (D_Bottom_TextureTarget)
		{
			D_Bottom_CaptureComponent2D->TextureTarget = D_Bottom_TextureTarget;
		}


		if (R_Left_TextureTarget)
		{
			R_Left_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
			R_Left_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
			R_Left_CaptureComponent2D->TextureTarget = R_Left_TextureTarget;
		}
		if (R_Right_TextureTarget)
		{
			R_Right_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
			R_Right_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
			R_Right_CaptureComponent2D->TextureTarget = R_Right_TextureTarget;
		}

		if (R_Top_TextureTarget)
		{
			R_Top_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
			R_Top_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
			R_Top_CaptureComponent2D->TextureTarget = R_Top_TextureTarget;
		}
		if (R_Bottom_TextureTarget)
		{
			R_Bottom_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
			R_Bottom_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
			R_Bottom_CaptureComponent2D->TextureTarget = R_Bottom_TextureTarget;
		}


		D_Left_CaptureComponent2D->OrthoWidth = Size;
		D_Right_CaptureComponent2D->OrthoWidth = Size;
		R_Left_CaptureComponent2D->OrthoWidth = Size;
		R_Right_CaptureComponent2D->OrthoWidth = Size;

		D_Top_CaptureComponent2D->OrthoWidth = Size;
		D_Bottom_CaptureComponent2D->OrthoWidth = Size;
		R_Top_CaptureComponent2D->OrthoWidth = Size;
		R_Bottom_CaptureComponent2D->OrthoWidth = Size;


		D_Left_CaptureComponent2D->SetRelativeLocation(FVector(0, -1.0f * Size / 2.0f, 0));
		D_Right_CaptureComponent2D->SetRelativeLocation(FVector(0, Size / 2.0f, 0));
		R_Left_CaptureComponent2D->SetRelativeLocation(FVector(0, -1.0f * Size / 2.0f, 0));
		R_Right_CaptureComponent2D->SetRelativeLocation(FVector(0, Size / 2.0f, 0));

		D_Top_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, Size / 2.0f));
		D_Bottom_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, -1.0f * Size / 2.0f));
		R_Top_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, Size / 2.0f));
		R_Bottom_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, -1.0f * Size / 2.0f));
	}

	/* --------------------------------------------------------------------------------------------*/

	return FReply::Handled();
}


FReply ACapture3DTool::Create() {
	if ((!ShowOnlyActorsR.IsEmpty()  && D_Front_CaptureComponent2D && D_Back_CaptureComponent2D && R_Front_CaptureComponent2D && 
		R_Back_CaptureComponent2D && D_Front_CaptureComponent2D->OrthoWidth && D_Back_CaptureComponent2D->OrthoWidth &&
		R_Front_CaptureComponent2D->OrthoWidth && R_Back_CaptureComponent2D->OrthoWidth) || (!ShowOnlyActorsR.IsEmpty() && 
			D_Left_CaptureComponent2D && D_Right_CaptureComponent2D && R_Left_CaptureComponent2D && R_Right_CaptureComponent2D && 
			D_Top_CaptureComponent2D && D_Bottom_CaptureComponent2D && R_Top_CaptureComponent2D && R_Bottom_CaptureComponent2D && 
			D_Left_CaptureComponent2D->OrthoWidth && D_Right_CaptureComponent2D->OrthoWidth && R_Left_CaptureComponent2D->OrthoWidth && 
			R_Right_CaptureComponent2D->OrthoWidth && D_Top_CaptureComponent2D->OrthoWidth && D_Bottom_CaptureComponent2D->OrthoWidth &&
			R_Top_CaptureComponent2D->OrthoWidth && R_Bottom_CaptureComponent2D->OrthoWidth &&
			PrintMethod== PM_Print3D_Plus || PM_Print3D_Plus_Halton))
	{
		AutoCreateTex();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Auto Create Error"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Auto Create Error"));
	}

	return FReply::Handled();
}



void ACapture3DTool::AutoCreateTex() {

	if (D_Front_CaptureComponent2D && D_Back_CaptureComponent2D && D_Front_CaptureComponent2D->TextureTarget && D_Back_CaptureComponent2D->TextureTarget &&
		R_Front_CaptureComponent2D && R_Back_CaptureComponent2D && R_Front_CaptureComponent2D->TextureTarget && R_Back_CaptureComponent2D->TextureTarget)
	{
		UTextureRenderTarget2D* D_Front_RenderTarget = D_Front_TextureTarget;
		UTextureRenderTarget2D* D_Back_RenderTarget = D_Back_TextureTarget;
		UTextureRenderTarget2D* R_Front_RenderTarget = R_Front_TextureTarget;
		UTextureRenderTarget2D* R_Back_RenderTarget = R_Back_TextureTarget;

		UTextureRenderTarget2D* D_Left_RenderTarget = NULL;
		UTextureRenderTarget2D* D_Right_RenderTarget = NULL;
		UTextureRenderTarget2D* R_Left_RenderTarget = NULL;
		UTextureRenderTarget2D* R_Right_RenderTarget = NULL;

		UTextureRenderTarget2D* D_Top_RenderTarget = NULL;
		UTextureRenderTarget2D* D_Bottom_RenderTarget = NULL;
		UTextureRenderTarget2D* R_Top_RenderTarget = NULL;
		UTextureRenderTarget2D* R_Bottom_RenderTarget = NULL;

		if (PrintMethod == PM_Print3D_Plus || PM_Print3D_Plus_Halton)
		{
			D_Left_RenderTarget = D_Left_TextureTarget;
			D_Right_RenderTarget = D_Right_TextureTarget;
			R_Left_RenderTarget = R_Left_TextureTarget;
			R_Right_RenderTarget = R_Right_TextureTarget;

			D_Top_RenderTarget = D_Top_TextureTarget;
			D_Bottom_RenderTarget = D_Bottom_TextureTarget;
			R_Top_RenderTarget = R_Top_TextureTarget;
			R_Bottom_RenderTarget = R_Back_TextureTarget;
		}

		Data D_data,R_data_F,R_data_B;
		Data D_data_Left, D_data_Top,R_data_Left_F, R_data_Left_B, R_data_Top_F, R_data_Top_B;

		TArray<FLinearColor> RGB_L_F;
		TArray<FLinearColor> RGB_L_B;

		TArray<FLinearColor> RGB_Left;
		TArray<FLinearColor> RGB_Right;
		TArray<FLinearColor> RGB_Top;
		TArray<FLinearColor> RGB_Bottom;

		FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
		ReadPixelFlags.SetLinearToGamma(true);

		D_data.SetDistance(Size);
		R_data_F.SetDistance(Size);
		R_data_B.SetDistance(Size);
		D_data_Top.SetDistance(Size);
		D_data_Left.SetDistance(Size);
		R_data_Left_F.SetDistance(Size);
		R_data_Left_B.SetDistance(Size);
		R_data_Top_F.SetDistance(Size);
		R_data_Top_B.SetDistance(Size);

		UWorld* world = this->GetWorld();
		if (PrintMethod == PM_Print3D_Plus || PM_Print3D_Plus_Halton)
		{
			R32fTool::EnterRender(D_Front_RenderTarget, D_Back_RenderTarget,D_Left_RenderTarget,D_Right_RenderTarget,D_Top_RenderTarget,D_Bottom_RenderTarget, world, D_data,D_data_Left,D_data_Top);
		}
		else {
			R32fTool::EnterRender(D_Front_RenderTarget, D_Back_RenderTarget, world, D_data);
		}
		R_Front_RenderTarget->GameThread_GetRenderTargetResource()->ReadLinearColorPixels(RGB_L_F, ReadPixelFlags);
		R_Back_RenderTarget->GameThread_GetRenderTargetResource()->ReadLinearColorPixels(RGB_L_B, ReadPixelFlags);
		R_Left_RenderTarget->GameThread_GetRenderTargetResource()->ReadLinearColorPixels(RGB_Left, ReadPixelFlags);
		R_Right_RenderTarget->GameThread_GetRenderTargetResource()->ReadLinearColorPixels(RGB_Right, ReadPixelFlags);
		R_Top_RenderTarget->GameThread_GetRenderTargetResource()->ReadLinearColorPixels(RGB_Top, ReadPixelFlags);
		R_Bottom_RenderTarget->GameThread_GetRenderTargetResource()->ReadLinearColorPixels(RGB_Bottom, ReadPixelFlags);

		R_data_F.DepthData.Append(RGB_L_F);
		R_data_B.DepthData.Append(RGB_L_B);
		R_data_Left_F.DepthData.Append(RGB_Left);
		R_data_Left_B.DepthData.Append(RGB_Right);
		R_data_Top_F.DepthData.Append(RGB_Top);
		R_data_Top_B.DepthData.Append(RGB_Bottom);
		FString name = this->GetClass()->GetName();
		FString gamepath = FPaths::GetBaseFilename(this->GetClass()->GetPathName(), false);
		int32 i;
		gamepath.FindLastChar('/', i);
		gamepath = gamepath.Mid(0, i);
		/*gamepath.RemoveFromEnd(TEXT("/") + name);*/
		UE_LOG(LogTemp, Warning, TEXT("Path: %s Name: %s"), *gamepath, *name);

		R_data_F.SetWidth(R_Front_RenderTarget->SizeX);
		R_data_F.SetHeight(R_Front_RenderTarget->SizeY);
		R_data_B.SetWidth(R_Front_RenderTarget->SizeX);
		R_data_B.SetHeight(R_Front_RenderTarget->SizeY);

		R_data_Left_F.SetWidth(R_Left_RenderTarget->SizeX);
		R_data_Left_F.SetHeight(R_Left_RenderTarget->SizeY);
		R_data_Left_B.SetWidth(R_Right_RenderTarget->SizeX);
		R_data_Left_B.SetHeight(R_Right_RenderTarget->SizeY);

		R_data_Top_F.SetWidth(R_Top_RenderTarget->SizeX);
		R_data_Top_F.SetHeight(R_Top_RenderTarget->SizeY);
		R_data_Top_B.SetWidth(R_Bottom_RenderTarget->SizeX);
		R_data_Top_B.SetHeight(R_Bottom_RenderTarget->SizeY);

		R32fTool::CreateNewStatic(gamepath, TEXT("Depth_FrontBack_") + name + TEXT("_Texture"), ECode::EC_RG, D_data);
		R32fTool::CreateNewStatic(gamepath, TEXT("Depth_LeftRight_") + name + TEXT("_Texture"), ECode::EC_RG, D_data_Left);
		R32fTool::CreateNewStatic(gamepath, TEXT("Depth_TopBottom_") + name + TEXT("_Texture"), ECode::EC_RG, D_data_Top);
		R32fTool::CreateNewStatic(gamepath, TEXT("RGB_Front_") + name + TEXT("_Texture"), ECode::EC_LDR, R_data_F);
		R32fTool::CreateNewStatic(gamepath, TEXT("RGB_Back_") + name + TEXT("_Texture"), ECode::EC_LDR, R_data_B);
		R32fTool::CreateNewStatic(gamepath, TEXT("RGB_Left_") + name + TEXT("_Texture"), ECode::EC_LDR, R_data_Left_F);
		R32fTool::CreateNewStatic(gamepath, TEXT("RGB_Right_") + name + TEXT("_Texture"), ECode::EC_LDR, R_data_Left_B);
		R32fTool::CreateNewStatic(gamepath, TEXT("RGB_Top_") + name + TEXT("_Texture"), ECode::EC_LDR, R_data_Top_F);
		R32fTool::CreateNewStatic(gamepath, TEXT("RGB_Bottom_") + name + TEXT("_Texture"), ECode::EC_LDR, R_data_Top_B);
		
		if (bAutoMaterial)
		{
			AutoCreateMaterial(gamepath, gamepath + TEXT("/Depth") + name + TEXT("_Texture"), gamepath + TEXT("/RGB_Front_") + name + TEXT("_Texture"));
		}
	}
}

void ACapture3DTool::AutoCreateMaterial(FString gamepath, FString DepthPath, FString TexPath) {
	FString Name = "Material_3DPrint";
	gamepath += Name;
	UPackage* package = CreatePackage(*gamepath);

	auto MatetialFactory = NewObject<UMaterialFactoryNew>();
	//UMaterial* Material = (UMaterial*)MatetialFactory->FactoryCreateNew(UMaterial::StaticClass(), package, *Name, RF_Standalone|RF_Public,NULL, GWarn);
	UMaterial * Material = (UMaterial*)MatetialFactory->FactoryCreateNew(UMaterial::StaticClass(), package, *Name, RF_Standalone | RF_Public, NULL, GWarn);
	FAssetRegistryModule::AssetCreated(Material);
	package->FullyLoad();
	package->SetDirtyFlag(true);
	UMaterialExpressionCustom* custom = NewObject<UMaterialExpressionCustom>(Material);
	UMaterialExpressionTextureCoordinate* textureCoordinate = NewObject<UMaterialExpressionTextureCoordinate>(Material);
	UMaterialExpressionCameraVectorWS* CameraVec = NewObject<UMaterialExpressionCameraVectorWS>(Material);
	//UMaterialExpressionScreenPosition* pixel = NewObject<UMaterialExpressionScreenPosition>(Material);
	
	UMaterialExpressionTextureObject* TexObj = NewObject<UMaterialExpressionTextureObject>(Material);
	UMaterialExpressionTextureObject* TexSample = NewObject<UMaterialExpressionTextureObject>(Material);
	UMaterialExpressionConstant* steps = NewObject<UMaterialExpressionConstant>(Material);
	UMaterialExpressionDDX* DDX = NewObject<UMaterialExpressionDDX>(Material);
	UMaterialExpressionDDY* DDY = NewObject<UMaterialExpressionDDY>(Material);
	UMaterialExpressionConstant4Vector* Background = NewObject<UMaterialExpressionConstant4Vector>(Material);
	UMaterialExpressionTransform* transform = NewObject<UMaterialExpressionTransform>(Material);
	UMaterialExpressionWorldPosition* Worldpos = nullptr;

	if (PrintMethod==PM_Print3D_Halton)
	{
		Worldpos = NewObject<UMaterialExpressionWorldPosition>(Material);
	}
	
	transform->TransformSourceType = TEnumAsByte<enum EMaterialVectorCoordTransformSource>::EnumType::TRANSFORMSOURCE_World;
	transform->TransformType = TEnumAsByte<enum EMaterialVectorCoordTransform>::EnumType::TRANSFORM_Tangent;

	UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *DepthPath);

	UTexture* DepthTex = Cast<UTexture>(FStringAssetReference(DepthPath).TryLoad());
	UTexture* FrontTex = Cast<UTexture>(FStringAssetReference(TexPath).TryLoad());
	
	steps->R = 16.0f;

	custom->OutputType = TEnumAsByte<enum ECustomMaterialOutputType>::EnumType::CMOT_Float4;
	custom->Inputs.Add(FCustomInput());
	custom->Inputs.Add(FCustomInput());
	custom->Inputs.Add(FCustomInput());
	custom->Inputs.Add(FCustomInput());
	custom->Inputs.Add(FCustomInput());
	custom->Inputs.Add(FCustomInput());
	custom->Inputs.Add(FCustomInput());
	custom->Inputs.Add(FCustomInput());
	custom->Inputs[0].InputName = TEXT("InUV");
	custom->Inputs[1].InputName = TEXT("CameraVec");
	custom->Inputs[2].InputName = TEXT("Steps");
	custom->Inputs[3].InputName = TEXT("PixelPos");
	custom->Inputs[4].InputName = TEXT("InDDX");
	custom->Inputs[5].InputName = TEXT("InDDY");
	custom->Inputs[6].InputName = TEXT("Depth");
	custom->Inputs[7].InputName = TEXT("Tex");
	custom->Inputs[8].InputName = TEXT("Background");
	custom->AdditionalOutputs.Add(FCustomOutput());
	custom->AdditionalOutputs.Add(FCustomOutput());
	custom->AdditionalOutputs[0].OutputName = TEXT("OutUV");
	custom->AdditionalOutputs[0].OutputType = TEnumAsByte<enum ECustomMaterialOutputType>::EnumType::CMOT_Float2;
	custom->AdditionalOutputs[1].OutputName = TEXT("Mask");
	custom->AdditionalOutputs[1].OutputType = TEnumAsByte<enum ECustomMaterialOutputType>::EnumType::CMOT_Float1;
	custom->IncludeFilePaths.Add("/Plugins/RT2DOutput/Print3D.ush");
	if (PrintMethod == PM_Print3D_Halton) {
		custom->Code = TEXT("float3 Info = Print3D_Halton(InUV,CameraVec,Steps,PixelPos,InDDX,InDDY,Depth,TexSampler);OutUV = Info.xy;Mask = Info.z;return Sampler(Tex,Info,Background,TexSampler);");
	}
	else if (PrintMethod == PM_Print3D) {
		custom->Code = TEXT("float3 Info = Print3D(InUV,CameraVec,Steps,PixelPos,InDDX,InDDY,Depth,TexSampler);OutUV = Info.xy;Mask = Info.z;return Sampler(Tex,Info,Background,TexSampler);");
	}

	transform->Input.Expression = CameraVec;


	custom->Inputs[0].Input.Expression = textureCoordinate;
	custom->Inputs[1].Input.Expression = transform;
	custom->Inputs[2].Input.Expression = steps;
	//custom->Inputs[3].Input.Expression = pixelpos;
	if (PrintMethod == PM_Print3D_Halton && Worldpos!=nullptr) {
		custom->Inputs[3].Input.Expression = Worldpos;
	}
	custom->Inputs[4].Input.Expression = DDX;
	custom->Inputs[5].Input.Expression = DDY;
	custom->Inputs[6].Input.Expression = TexObj;
	custom->Inputs[7].Input.Expression = TexSample;
	custom->Inputs[8].Input.Expression = Background;

	DDX->Value.Expression = textureCoordinate;
	DDY->Value.Expression = textureCoordinate;


	Material->Expressions.Add(textureCoordinate);
	Material->Expressions.Add(CameraVec);
	Material->Expressions.Add(steps);
	Material->Expressions.Add(DDX);
	Material->Expressions.Add(DDY);
	Material->Expressions.Add(TexObj);
	Material->Expressions.Add(TexSample);
	Material->Expressions.Add(Background);
	Material->Expressions.Add(custom);
	Material->Expressions.Add(transform);
	
	if (PrintMethod == PM_Print3D_Halton && Worldpos != nullptr) {
		Material->Expressions.Add(Worldpos);
	}

	if (DepthTex)
	{
		TexObj->Texture = DepthTex;
		TexObj->SamplerType = TEnumAsByte<enum EMaterialSamplerType>::EnumType::SAMPLERTYPE_Normal;
	}
	if (FrontTex)
	{
		TexSample->Texture = FrontTex;
		TexSample->SamplerType = TEnumAsByte<enum EMaterialSamplerType>::EnumType::SAMPLERTYPE_LinearColor;
	}
	Material->BaseColor.Expression = custom;

	Material->PreEditChange(NULL);
	Material->PostEditChange();

}


#if WITH_EDITOR
void ACapture3DTool::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!ShowOnlyActorsR.IsEmpty() || LastShowOnlyActorsR != ShowOnlyActorsR) {
		D_Front_CaptureComponent2D->ShowOnlyActors.Empty();
		D_Back_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Front_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Back_CaptureComponent2D->ShowOnlyActors.Empty();
		/* Plus----------------------------------------------------------------------------------------*/
		D_Left_CaptureComponent2D->ShowOnlyActors.Empty();
		D_Right_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Left_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Right_CaptureComponent2D->ShowOnlyActors.Empty();


		D_Top_CaptureComponent2D->ShowOnlyActors.Empty();
		D_Bottom_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Top_CaptureComponent2D->ShowOnlyActors.Empty();
		R_Bottom_CaptureComponent2D->ShowOnlyActors.Empty();

		/* --------------------------------------------------------------------------------------------*/

		LastShowOnlyActorsR.Empty();

		D_Front_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		LastShowOnlyActorsR.Append(ShowOnlyActorsR);
		D_Back_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Front_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Back_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		/* Plus----------------------------------------------------------------------------------------*/
		D_Left_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		D_Right_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Left_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Right_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);

		D_Top_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		D_Bottom_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Top_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		R_Bottom_CaptureComponent2D->ShowOnlyActors.Append(ShowOnlyActorsR);
		/* --------------------------------------------------------------------------------------------*/
	}

		this->SetActorRelativeScale3D(FVector(1, 1, 1));

		Box->SetRelativeScale3D(FVector(Size / 2.0f, Size / 2.0f, Size / 2.0f));


		if (D_Front_TextureTarget && D_Back_TextureTarget)
		{
			D_Front_CaptureComponent2D->TextureTarget = D_Front_TextureTarget;
		}

		if (D_Back_TextureTarget)
		{
			D_Back_CaptureComponent2D->TextureTarget = D_Back_TextureTarget;
		}

		if (R_Front_TextureTarget)
		{
			R_Front_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
			R_Front_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
			R_Front_CaptureComponent2D->TextureTarget = R_Front_TextureTarget;
		}

		if (R_Back_TextureTarget)
		{
			R_Back_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
			R_Back_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
			R_Back_CaptureComponent2D->TextureTarget = R_Back_TextureTarget;
		}

		D_Front_CaptureComponent2D->OrthoWidth = Size;
		D_Back_CaptureComponent2D->OrthoWidth = Size;
		R_Front_CaptureComponent2D->OrthoWidth = Size;
		R_Back_CaptureComponent2D->OrthoWidth = Size;

		D_Front_CaptureComponent2D->SetRelativeLocation(FVector(-1.0f * Size / 2.0f, 0, 0));
		D_Back_CaptureComponent2D->SetRelativeLocation(FVector(Size / 2.0f, 0, 0));
		R_Front_CaptureComponent2D->SetRelativeLocation(FVector(-1.0f * Size / 2.0f, 0, 0));
		R_Back_CaptureComponent2D->SetRelativeLocation(FVector(Size / 2.0f, 0, 0));

		/* Plus----------------------------------------------------------------------------------------*/
		if (PrintMethod == PM_Print3D_Plus || PM_Print3D_Plus_Halton)
		{

			if (D_Left_TextureTarget)
			{
				D_Left_CaptureComponent2D->TextureTarget = D_Left_TextureTarget;
			}
			if (D_Right_TextureTarget)
			{
				D_Right_CaptureComponent2D->TextureTarget = D_Right_TextureTarget;
			}

			if (D_Top_TextureTarget)
			{
				D_Left_CaptureComponent2D->TextureTarget = D_Top_TextureTarget;
			}
			if (D_Bottom_TextureTarget)
			{
				D_Bottom_CaptureComponent2D->TextureTarget = D_Bottom_TextureTarget;
			}


			if (R_Left_TextureTarget)
			{
				R_Left_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
				R_Left_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
				R_Left_CaptureComponent2D->TextureTarget = R_Left_TextureTarget;
			}
			if (R_Right_TextureTarget)
			{
				R_Right_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
				R_Right_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
				R_Right_CaptureComponent2D->TextureTarget = R_Right_TextureTarget;
			}

			if (R_Top_TextureTarget)
			{
				R_Top_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
				R_Top_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
				R_Top_CaptureComponent2D->TextureTarget = R_Top_TextureTarget;
			}
			if (R_Bottom_TextureTarget)
			{
				R_Bottom_TextureTarget->InitCustomFormat(512, 512, EPixelFormat::PF_R8G8B8A8, true);
				R_Bottom_TextureTarget->RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_RGBA8;
				R_Bottom_CaptureComponent2D->TextureTarget = R_Bottom_TextureTarget;
			}


			D_Left_CaptureComponent2D->OrthoWidth = Size;
			D_Right_CaptureComponent2D->OrthoWidth = Size;
			R_Left_CaptureComponent2D->OrthoWidth = Size;
			R_Right_CaptureComponent2D->OrthoWidth = Size;

			D_Top_CaptureComponent2D->OrthoWidth = Size;
			D_Bottom_CaptureComponent2D->OrthoWidth = Size;
			R_Top_CaptureComponent2D->OrthoWidth = Size;
			R_Bottom_CaptureComponent2D->OrthoWidth = Size;


			D_Left_CaptureComponent2D->SetRelativeLocation(FVector(0, -1.0f * Size / 2.0f, 0));
			D_Right_CaptureComponent2D->SetRelativeLocation(FVector(0, Size / 2.0f, 0));
			R_Left_CaptureComponent2D->SetRelativeLocation(FVector(0, -1.0f * Size / 2.0f, 0));
			R_Right_CaptureComponent2D->SetRelativeLocation(FVector(0, Size / 2.0f, 0));

			D_Top_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, Size / 2.0f));
			D_Bottom_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, -1.0f * Size / 2.0f));
			R_Top_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, Size / 2.0f));
			R_Bottom_CaptureComponent2D->SetRelativeLocation(FVector(0, 0, -1.0f * Size / 2.0f));
		}

		/* --------------------------------------------------------------------------------------------*/
}
#endif
