// ProjectSurfing © 2023 Furkan Gülmez. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UpscalerSwitcherTypes.h"
#include "GameFramework/GameUserSettings.h"
#include "DLSSLibrary.h"
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
	UFUNCTION(BlueprintCallable, BlueprintPure) FORCEINLINE
	EUpscaler GetCurrentUpscaler() const{ return CurrentUpscaler;}

	UFUNCTION(BlueprintCallable) FORCEINLINE
	void SetCurrentUpscaler(EUpscaler NewUpscaler){ CurrentUpscaler = NewUpscaler;};

	UFUNCTION(BlueprintCallable) FORCEINLINE
	void SetDLSSQualityMode(UDLSSMode NewDLSSQualityMode){ DLSSQualityMode = NewDLSSQualityMode;};
	
	UFUNCTION(BlueprintCallable, BlueprintPure) FORCEINLINE
	UDLSSMode GetDLSSQualityMode() const{ return DLSSQualityMode;};
	
	UFUNCTION(BlueprintCallable) FORCEINLINE
	void SetDLSSFrameGenEnabled(bool NewDLSSFrameGenEnabled){ bDLSSFrameGenEnabled = NewDLSSFrameGenEnabled;};

	UFUNCTION(BlueprintCallable, BlueprintPure) FORCEINLINE
	bool GetDLSSFrameGenEnabled() const{ return bDLSSFrameGenEnabled;};

	UFUNCTION(BlueprintCallable) FORCEINLINE
	void SetFSRQualityMode(EFFXFSR3QualityMode_Custom NewFSRQualityMode){ FSRQualityMode = NewFSRQualityMode;};
	
	UFUNCTION(BlueprintCallable, BlueprintPure) FORCEINLINE
	EFFXFSR3QualityMode_Custom GetFSRQualityMode() const{ return FSRQualityMode;};

	UFUNCTION(BlueprintCallable) FORCEINLINE
	void SetFSRFrameGenEnabled(bool NewFSRFrameGenEnabled) { bFSRFrameGenEnabled = NewFSRFrameGenEnabled;};

	UFUNCTION(BlueprintCallable, BlueprintPure) FORCEINLINE
	bool GetFSRFrameGenEnabled() const{return bFSRFrameGenEnabled;};
private:
	UPROPERTY(Config)
	EUpscaler CurrentUpscaler;

	UPROPERTY(config)
	UDLSSMode DLSSQualityMode;

	UPROPERTY(config)
	bool bDLSSFrameGenEnabled;

	UPROPERTY(config)
	EFFXFSR3QualityMode_Custom FSRQualityMode;

	UPROPERTY(config)
	bool bFSRFrameGenEnabled;
	
};
