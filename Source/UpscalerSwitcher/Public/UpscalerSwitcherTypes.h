// Penguru Games © 2025 Mehmet Furkan Gülmez. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UpscalerSwitcherTypes.generated.h"

UENUM(BlueprintType)
enum class EUpscaler : uint8
{
	None,
	DLSS,
	FSR
};

UENUM(BlueprintType)
enum class EFFXFSR3QualityMode_Custom : uint8
{
	NativeAA,
	Quality,
	Balanced,
	Performance,
	UltraPerformance,
};

UENUM(BlueprintType)
enum class UDLSSMode_Custom : uint8
{
	Off              UMETA(DisplayName = "Off"),
	Auto             UMETA(DisplayName = "Auto", ToolTip = "Not a real quality mode. Use Auto to query best settings for a given resolution with GetDLSSModeInformation"),
	DLAA             UMETA(DisplayName = "DLAA"),
	UltraQuality     UMETA(DisplayName = "Ultra Quality"),
	Quality          UMETA(DisplayName = "Quality"),
	Balanced         UMETA(DisplayName = "Balanced"),
	Performance      UMETA(DisplayName = "Performance"),
	UltraPerformance UMETA(DisplayName = "Ultra Performance")
};

USTRUCT(BlueprintType)
struct FFSRModeInformation
{
	GENERATED_BODY()
public:
	FFSRModeInformation()
	{
		Quality = EFFXFSR3QualityMode_Custom::NativeAA;
		bFrameGenEnabled = false;
	};
	
	FFSRModeInformation(const EFFXFSR3QualityMode_Custom Quality, const bool FrameGenEnabled):
	Quality(Quality), bFrameGenEnabled(FrameGenEnabled)
	{}
	UPROPERTY(BlueprintReadWrite)
	EFFXFSR3QualityMode_Custom Quality;

	UPROPERTY(BlueprintReadWrite)
	bool bFrameGenEnabled;
};


USTRUCT(BlueprintType)
struct FDLSSModeInformation
{
	GENERATED_BODY()
public:
	FDLSSModeInformation()
	{
		Quality = UDLSSMode_Custom::Off;
		bFrameGenEnabled = false;
		OptimalScreenPercentage = 100.0f;
	};
	FDLSSModeInformation(const UDLSSMode_Custom Quality, const float OptScreenPercentage,const bool FrameGenEnabled):
	Quality(Quality), OptimalScreenPercentage(OptScreenPercentage), bFrameGenEnabled(FrameGenEnabled)
	{}
	UPROPERTY(BlueprintReadWrite)
	UDLSSMode_Custom Quality;
	UPROPERTY(BlueprintReadWrite)
	float OptimalScreenPercentage;
	UPROPERTY(BlueprintReadWrite)
	bool bFrameGenEnabled;

	
	// UPROPERTY(BlueprintReadWrite)
	// bool bIsSupported;
	// UPROPERTY(BlueprintReadWrite)
	// bool bIsFixedScreenPercentage;
	// UPROPERTY(BlueprintReadWrite)
	// float MinScreenPercentage;
	// UPROPERTY(BlueprintReadWrite)
	// float MaxScreenPercentage;
	// UPROPERTY(BlueprintReadWrite)
	// float OptimalSharpness;
};
