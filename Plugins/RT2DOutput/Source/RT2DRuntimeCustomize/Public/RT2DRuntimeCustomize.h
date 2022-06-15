#pragma once

#include "Modules/ModuleManager.h"


class FRuntimeCustomizationModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};