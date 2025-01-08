#pragma once


#include "UpscalerSwitcherTypes.h"
#include "UObject/Object.h"
#include "DLSSLibrary.h"
#include "UpscalerSwitcherSettings.generated.h"

UCLASS(Config = Engine, DefaultConfig, DisplayName="Upscaler Switcher")
class UPSCALERSWITCHER_API UUpscalerSwitcherSettings : public UObject
{
	GENERATED_BODY()
public:
	/** Default upscaling method to be used in the engine (e.g., DLSS, FSR, etc.) */
	UPROPERTY(Config, EditAnywhere, Category = "Settings", DisplayName = "Default Upscaler Method")
	EUpscaler DefaultUpscaler = EUpscaler::None;

	/** Default DLSS mode to be applied (e.g., Quality, Performance)*/
	UPROPERTY(Config, EditAnywhere, Category = "Settings", DisplayName = "Default DLSS Mode")
	UDLSSMode DefaultDLSSMode = UDLSSMode::Quality;

	/** Enables or disables DLSS Frame Generation by default */
	UPROPERTY(Config, EditAnywhere, Category = "Settings", DisplayName = "Default DLSS Frame Gen Enabled")
	bool bDefaultDLSSFrameGen = false;

	/** Default FSR quality mode (e.g., Quality, Performance) */
	UPROPERTY(Config, EditAnywhere, Category = "Settings", DisplayName = "Default FSR Mode")
	EFFXFSR3QualityMode_Custom DefaultFSRQualityMode = EFFXFSR3QualityMode_Custom::Quality;

	/** Enables or disables FSR Frame Generation by default */
	UPROPERTY(Config, EditAnywhere, Category = "Settings", DisplayName = "Default FSR Frame Gen Enabled")
	bool bDefaultFSRFrameGen = false;
	
};
