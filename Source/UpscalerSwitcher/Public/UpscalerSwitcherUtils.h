// Penguru Games © 2025 Mehmet Furkan Gülmez. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UpscalerSwitcherTypes.h"
#include "UObject/Object.h"
#include "UpscalerSwitcherUtils.generated.h"

UCLASS(Abstract)
class UPSCALERSWITCHER_API UUpscalerSwitcherUtils : public UObject
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void ApplyDLSS(FDLSSModeInformation DLSSMode, bool bSaveConfig = true);

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void ApplyFSR(FFSRModeInformation FSRMode, bool bSaveConfig = true);

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void DisableUpscaling(bool bSaveConfig = true);
	
	/**
	 * Applies the current upscaler method that saved in the UpscalerGameUserSettings
	 */
	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
    static void ApplySavedUpscaler();
	//
	// UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	// static void UpdateDLSSModeInformation(FDLSSModeInformation DLSSMode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher")
	static EUpscaler GetSavedUpscaler();
	
	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void SetDLSSQuality(UDLSSMode_Custom Quality, float OptimalScreenPercentage, bool bSaveConfig = true, bool bApplyUpscaling = true);

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void SetFSRQuality(EFFXFSR3QualityMode_Custom Quality, bool bSaveConfig = true, bool bApplyUpscaling = true);

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void SetFSRFramegen(bool bFrameGenEnabled, bool bSaveConfig = true, bool bApplyUpscaling = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher")
	static bool GetFSREnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher")
	static bool GetDLSSEnabled();
private:
	
private:
	// These variables and functions are implemented here because the DLSS plugin cannot be used directly here
	// and the plugin would not compile without the DLSS plugin. 
	// I copied them from DLSS plugin. Could not figure out another way. 
	static int32 PreviousShadowDenoiser;
	static int32 PreviousLumenSSR;
	static int32 PreviousLumenTemporal;
	static bool bDenoisingRequested;
	
	static void EnableDLSS(bool bEnabled);
	static void EnableDLSSRR(bool bEnabled);
	static bool IsDLSSRREnabled();
	static bool IsDLSSEnabled();

	static TMap<UDLSSMode_Custom, float> DLSSQualityMap;
};
