// ProjectSurfing © 2023 Furkan Gülmez. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UpscalerSwitcherTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "UpscalerGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class UPSCALERSWITCHER_API UUpscalerGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UUpscalerGameUserSettings(const FObjectInitializer& ObjectInitializer);
	
	/** Returns the game local machine settings (resolution, windowing mode, scalability settings, etc...) */
	UFUNCTION(BlueprintCallable, Category=Settings)
	static UUpscalerGameUserSettings* GetUpscalerGameUserSettings();

	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	virtual void SetToDefaults() override;
	virtual void ValidateSettings() override;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher|Settings") FORCEINLINE
	EUpscaler GetCurrentUpscaler() const{ return CurrentUpscaler;}

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher|Settings") FORCEINLINE
	void SetCurrentUpscaler(EUpscaler NewUpscaler){ CurrentUpscaler = NewUpscaler;};

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher|Settings") FORCEINLINE
	void SetDLSSQualityMode(UDLSSMode_Custom NewDLSSQualityMode){ DLSSQualityMode = NewDLSSQualityMode;};
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher|Settings") FORCEINLINE
	UDLSSMode_Custom GetDLSSQualityMode() const{ return DLSSQualityMode;};
	
	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher|Settings") FORCEINLINE
	void SetDLSSFrameGenEnabled(bool NewDLSSFrameGenEnabled){ bDLSSFrameGenEnabled = NewDLSSFrameGenEnabled;};

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher|Settings") FORCEINLINE
	bool GetDLSSFrameGenEnabled() const{ return bDLSSFrameGenEnabled;};

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher|Settings") FORCEINLINE
	void SetDLSSOptimalScreenPercentage(float OptimalScreenPercentage){ DLSSOptimalScreenPercentage = OptimalScreenPercentage;};

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher|Settings") FORCEINLINE
	float GetDLSSOptimalScreenPercentage() const{ return DLSSOptimalScreenPercentage;};

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher|Settings") FORCEINLINE
	void SetFSRQualityMode(EFFXFSR3QualityMode_Custom NewFSRQualityMode){ FSRQualityMode = NewFSRQualityMode;};
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher|Settings") FORCEINLINE
	EFFXFSR3QualityMode_Custom GetFSRQualityMode() const{ return FSRQualityMode;};

	UFUNCTION(BlueprintCallable, Category = "Upscaler Switcher|Settings") FORCEINLINE
	void SetFSRFrameGenEnabled(bool NewFSRFrameGenEnabled) { bFSRFrameGenEnabled = NewFSRFrameGenEnabled;};

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upscaler Switcher|Settings") FORCEINLINE
	bool GetFSRFrameGenEnabled() const{return bFSRFrameGenEnabled;};
private:
	UPROPERTY(Config)
	EUpscaler CurrentUpscaler;

	UPROPERTY(Config)
	UDLSSMode_Custom DLSSQualityMode;

	UPROPERTY(Config)
	float DLSSOptimalScreenPercentage;

	UPROPERTY(Config)
	bool bDLSSFrameGenEnabled;

	UPROPERTY(Config)
	EFFXFSR3QualityMode_Custom FSRQualityMode;

	UPROPERTY(Config)
	bool bFSRFrameGenEnabled;
	
};
