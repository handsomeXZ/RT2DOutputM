#include "RT2DRuntimeCustomize.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"



IMPLEMENT_GAME_MODULE(FRuntimeCustomizationModule, RT2DRuntimeCustomize);

#define LOCTEXT_NAMESPACE "RT2DRuntimeCustomize"

void FRuntimeCustomizationModule::StartupModule() {
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("RT2DOutput"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugins/RT2DOutput"),PluginShaderDir);
}

void FRuntimeCustomizationModule::ShutdownModule() {
	ResetAllShaderSourceDirectoryMappings();
}


#undef LOCTEXT_NAMESPACE