// Penguru Games © 2025 Mehmet Furkan Gülmez. All Rights Reserved

#include "UpscalerSwitcher.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "UpscalerSwitcherSettings.h"

#define LOCTEXT_NAMESPACE "FUpscalerSwitcherModule"

void FUpscalerSwitcherModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "UpscalerSwitcher_Settings",
		                                 LOCTEXT("RuntimeSettingsName", "Upscaler Switcher"),
		                                 LOCTEXT("RuntimeSettingsDescription", "Configure upscaler switcher setting"),
		                                 GetMutableDefault<UUpscalerSwitcherSettings>());
	}
}

void FUpscalerSwitcherModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "UpscalerSwitcher_Settings");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUpscalerSwitcherModule, UpscalerSwitcher)