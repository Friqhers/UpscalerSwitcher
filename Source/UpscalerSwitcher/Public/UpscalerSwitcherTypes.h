#pragma once

#include "CoreMinimal.h"

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