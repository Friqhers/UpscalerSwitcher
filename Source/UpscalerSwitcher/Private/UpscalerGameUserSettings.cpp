// ProjectSurfing © 2023 Furkan Gülmez. All Rights Reserved


#include "UpscalerGameUserSettings.h"
#include "UpscalerSwitcherSettings.h"
#include "UpscalerSwitcherUtils.h"

UUpscalerGameUserSettings::UUpscalerGameUserSettings(const FObjectInitializer& ObjectInitializer)
{
	CurrentUpscaler = EUpscaler::None;
	DLSSQualityMode = UDLSSMode::Auto;
	bDLSSFrameGenEnabled = false;
	FSRQualityMode = EFFXFSR3QualityMode_Custom::Quality;
	bFSRFrameGenEnabled = false;
}

UUpscalerGameUserSettings* UUpscalerGameUserSettings::GetUpscalerGameUserSettings()
{
	if(UUpscalerGameUserSettings* UpscalerGameUserSettings = Cast<UUpscalerGameUserSettings>(GEngine->GetGameUserSettings()))
		return UpscalerGameUserSettings;
	return nullptr;
}

void UUpscalerGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	ApplyResolutionSettings(bCheckForCommandLineOverrides);
	ApplyNonResolutionSettings();

	UUpscalerSwitcherUtils::ApplyCurrentUpscalerMethod();
	
	RequestUIUpdate();
	SaveSettings();
}

void UUpscalerGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	const UUpscalerSwitcherSettings* Plugin_Settings = GetMutableDefault<UUpscalerSwitcherSettings>();
	
	CurrentUpscaler = Plugin_Settings ? Plugin_Settings->DefaultUpscaler : EUpscaler::None;
	DLSSQualityMode = Plugin_Settings ? Plugin_Settings->DefaultDLSSMode : UDLSSMode::Quality;
	bDLSSFrameGenEnabled = Plugin_Settings ? Plugin_Settings->bDefaultDLSSFrameGen : false;
	FSRQualityMode = Plugin_Settings ? Plugin_Settings->DefaultFSRQualityMode : EFFXFSR3QualityMode_Custom::Quality;
	bFSRFrameGenEnabled = Plugin_Settings ? Plugin_Settings->bDefaultFSRFrameGen : false;
}

void UUpscalerGameUserSettings::ValidateSettings()
{
	Super::ValidateSettings();
}
