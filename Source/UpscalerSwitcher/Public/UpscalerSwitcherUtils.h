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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool GetFSREnabled();
};
