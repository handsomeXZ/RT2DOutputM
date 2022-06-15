
#include "TextureRenderTarget2DR32f.h"


UTextureRenderTarget2DR32f::UTextureRenderTarget2DR32f(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RenderTargetFormat = TEnumAsByte<ETextureRenderTargetFormat>::EnumType::RTF_R32f;
// 	SizeX = 512;
// 	SizeY = 512;

}
