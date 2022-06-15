#pragma once

#include "Modules/ModuleManager.h"
#include "IAssetTypeActions.h"


class FEditorCustomizationModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RegisterAssetTools();
	void RegisterCustomDetials();
	void UnRegisterCustomDetials();
	void UnRegisterAssetTools();
private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};