// Copyright HandsomeCheese


#include "SceneCaptureComponent2DR32f.h"
#include "Camera/CameraTypes.h"
#include <Components/SceneCaptureComponent.h>
#include <Engine/EngineTypes.h>

USceneCaptureComponent2DR32f::USceneCaptureComponent2DR32f(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProjectionType = TEnumAsByte<ECameraProjectionMode::Type>::EnumType::Orthographic;
	PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	CaptureSource = TEnumAsByte<enum ESceneCaptureSource>::EnumType::SCS_SceneDepth;
}