#pragma once

#include "CoreMinimal.h"
#include "DLSSLibrary.h"
#include "UpscalerSwitcherTypes.h"
#include "UObject/Object.h"
#include "UpscalerSwitcherUtils.generated.h"

UCLASS(Abstract)
class UPSCALERSWITCHER_API UUpscalerSwitcherUtils : public UObject
{
public:
	GENERATED_BODY()
	
	/**
	* Applies the given upscaler method and also saves it to the UpscalerGameUserSettings if save config true
	*/
	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void SetUpscalerMethod(const EUpscaler Upscaler, bool bSaveConfig = true);

	/**
	 * Applies the current upscaler method that saved in the UpscalerGameUserSettings
	 */
	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void ApplyCurrentUpscalerMethod();

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void SetDLSSQuality(UDLSSMode Quality, bool bSaveConfig = true, bool bApplyUpscaling = true);

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void SetFSRQuality(EFFXFSR3QualityMode_Custom Quality, bool bSaveConfig = true, bool bApplyUpscaling = true);

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher")
	static void SetFSRFramegen(bool bFrameGenEnabled, bool bSaveConfig = true, bool bApplyUpscaling = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher")
	static bool GetFSREnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher")
	static bool GetDLSSEnabled();
};
