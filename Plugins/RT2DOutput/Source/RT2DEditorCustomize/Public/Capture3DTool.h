// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Capture3DTool.generated.h"

UENUM()
enum EPrintMethod {
	PM_Print3D,
	PM_Print3D_Halton,			/* beta */
	PM_Print3D_Plus,			/* beta */
	PM_Print3D_Plus_Halton		/* beta */
};

UCLASS()
class RT2DEDITORCUSTOMIZE_API ACapture3DTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACapture3DTool();

	

	/** The desired width (in world units) of the orthographic view (ignored in Perspective mode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DToolSceneCapture")
	float Size;

	/** Output Depth Front render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DToolSceneCapture")
	TObjectPtr<class UTextureRenderTarget2DR32f> D_Front_TextureTarget;
	/** Output Depth Back render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DToolSceneCapture")
	TObjectPtr<class UTextureRenderTarget2DR32f> D_Back_TextureTarget;

	/** Output LDR Front render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DToolSceneCapture")
	TObjectPtr<class UTextureRenderTarget2DR32f> R_Front_TextureTarget;
	/** Output LDR Back render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3DToolSceneCapture")
	TObjectPtr<class UTextureRenderTarget2DR32f> R_Back_TextureTarget;

	/** The only actors to be rendered by this scene capture, if PrimitiveRenderMode is set to UseShowOnlyList.*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "3DToolSceneCapture")
	TArray<TObjectPtr<AActor>> ShowOnlyActorsR;

	/** True£ºAuto Create Example Material£¨beta£© */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "3DToolSceneCapture")
	bool bAutoMaterial;

	/** Select method to create Material */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "3DToolSceneCapture")
	TEnumAsByte<EPrintMethod> PrintMethod = PM_Print3D;


	/** Output Depth Left render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> D_Left_TextureTarget;
	/** Output Depth Right render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> D_Right_TextureTarget;

	/** Output LDR Left render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> R_Left_TextureTarget;
	/** Output LDR Right render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> R_Right_TextureTarget;


	/** Output Depth Top render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> D_Top_TextureTarget;
	/** Output Depth Bottom render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> D_Bottom_TextureTarget;

	/** Output LDR Top render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> R_Top_TextureTarget;
	/** Output LDR Bottom render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plus")
		TObjectPtr<class UTextureRenderTarget2DR32f> R_Bottom_TextureTarget;


// 	UPROPERTY(Category = "3DToolSceneCapture", VisibleAnywhere)
// 	UTextureRenderTarget2DR32f* testRT;
	//virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	class FReply Refresh();
	class FReply Create();

	void AutoCreateTex();
	void AutoCreateMaterial(FString gamepath, FString DepthPath, FString TexPath);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> LastShowOnlyActorsR;

	UPROPERTY()
	class	USceneCaptureComponent2DR32f* D_Back_CaptureComponent2D;
	UPROPERTY()
	class	USceneCaptureComponent2DR32f* D_Front_CaptureComponent2D;

	UPROPERTY()
	class	USceneCaptureComponent2DR32f* R_Back_CaptureComponent2D;
	UPROPERTY()
	class	USceneCaptureComponent2DR32f* R_Front_CaptureComponent2D;

	UPROPERTY()
		class	USceneCaptureComponent2DR32f* D_Left_CaptureComponent2D;
	UPROPERTY()
		class	USceneCaptureComponent2DR32f* D_Right_CaptureComponent2D;

	UPROPERTY()
		class	USceneCaptureComponent2DR32f* R_Left_CaptureComponent2D;
	UPROPERTY()
		class	USceneCaptureComponent2DR32f* R_Right_CaptureComponent2D;


	UPROPERTY()
		class	USceneCaptureComponent2DR32f* D_Top_CaptureComponent2D;
	UPROPERTY()
		class	USceneCaptureComponent2DR32f* D_Bottom_CaptureComponent2D;

	UPROPERTY()
		class	USceneCaptureComponent2DR32f* R_Top_CaptureComponent2D;
	UPROPERTY()
		class	USceneCaptureComponent2DR32f* R_Bottom_CaptureComponent2D;

	UPROPERTY(Category = "Box", VisibleAnywhere)
	class	UBoxComponent* Box;


	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;


};
