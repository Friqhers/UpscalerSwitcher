// Penguru Games © 2025 Mehmet Furkan Gülmez. All Rights Reserved

#include "UpscalerSwitcherUtils.h"
#include "UpscalerGameUserSettings.h"
//#include "Runtime/Core/Public/HAL/IConsoleManager.h"
#include "HAL/IConsoleManager.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogUpscalerUtils, Log, All);

//FDLSSModeInformation UUpscalerSwitcherUtils::DLSSModeInformation = FDLSSModeInformation();
TMap<UDLSSMode_Custom, float> UUpscalerSwitcherUtils::DLSSQualityMap = {
	{ UDLSSMode_Custom::Off, 100.0f },
	//{ UDLSSMode_Custom::Auto, 1.0f },
	//{ UDLSSMode_Custom::DLAA, 2.0f },
	{ UDLSSMode_Custom::UltraQuality, 75.0f },
	{ UDLSSMode_Custom::Quality, 66.7f },
	{ UDLSSMode_Custom::Balanced, 58.0f },
	{ UDLSSMode_Custom::Performance, 50.0f },
	{ UDLSSMode_Custom::UltraPerformance, 33.3f }
};

int32 UUpscalerSwitcherUtils::PreviousShadowDenoiser = 1;
int32 UUpscalerSwitcherUtils::PreviousLumenSSR = 1;
int32 UUpscalerSwitcherUtils::PreviousLumenTemporal = 1;
bool UUpscalerSwitcherUtils::bDenoisingRequested = false;


void UUpscalerSwitcherUtils::ApplyDLSS(FDLSSModeInformation DLSSMode, bool bSaveConfig)
{

	const FString CommandReset = "r.ScreenPercentage.Reset";
	static IConsoleVariable* ScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));

	
	// disable fsr upscaling	
	GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FSR3.Enabled 0"));
	// disable fsr frame gen
	GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FI.Enabled 0"));

	// enable dlss
	//UDLSSLibrary::EnableDLSS(true);
	EnableDLSS(true);
	if(!IsDLSSEnabled())
	{
		UE_LOG(LogUpscalerUtils, Warning,
			TEXT("%s - Could not enabled DLSS!"),
			ANSI_TO_TCHAR(__FUNCTION__));
		return;
	};
			
	if(ScreenPercentage)
	{
		ScreenPercentage->Set(DLSSMode.OptimalScreenPercentage);
		UE_LOG(LogUpscalerUtils, Display,
			TEXT("%s - ScreenPercentage set to %s with IConsoleVariable"),
			ANSI_TO_TCHAR(__FUNCTION__), *FString::SanitizeFloat(DLSSMode.OptimalScreenPercentage));
		// if(const float* ResValue = DLSSQualityMap.Find(DLSSMode.Quality))
		// {
		// 	UE_LOG(LogTemp, Display,
		// 		TEXT("UUpscalerSwitcherUtils::SetUpscalerMethod -> ScreenPercentage reset to %s with IConsoleVariable"),
		// 		*FString::SanitizeFloat(*ResValue));
		// 	ScreenPercentage->Set(*ResValue);
		// }
	}
	
	if(bSaveConfig)
	{
		UE_LOG(LogTemp, Display, TEXT("%s -> Saving..."), ANSI_TO_TCHAR(__FUNCTION__));
		if(UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings())
		{
			Settings->SetCurrentUpscaler(EUpscaler::DLSS);
			Settings->SetDLSSQualityMode(DLSSMode.Quality);
			Settings->SetDLSSFrameGenEnabled(DLSSMode.bFrameGenEnabled);
			Settings->SetDLSSOptimalScreenPercentage(DLSSMode.OptimalScreenPercentage);
			Settings->SaveSettings();
			UE_LOG(LogTemp, Display, TEXT("%s - Save completed!"), ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}
		UE_LOG(LogTemp, Display, TEXT("%s - Save failed!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void UUpscalerSwitcherUtils::ApplyFSR(FFSRModeInformation FSRMode, bool bSaveConfig)
{
	// disable dlss
	//UDLSSLibrary::EnableDLSS(false);
	EnableDLSS(false);
	if(IsDLSSEnabled())
	{
		UE_LOG(LogUpscalerUtils, Warning,
			TEXT("%s - Could not disabled DLSS!"),
			ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	static IConsoleVariable* FSR3Enable = IConsoleManager::Get().FindConsoleVariable(TEXT("r.FidelityFX.FSR3.Enabled"));
	if(FSR3Enable)
	{
		FSR3Enable->Set(1, ECVF_SetByConsole);
		UE_LOG(LogUpscalerUtils, Display,
			TEXT("%s - Executed r.FidelityFX.FSR3.Enabled 1"),
			ANSI_TO_TCHAR(__FUNCTION__));
	}

	static IConsoleVariable* FSRFrameGenEnable = IConsoleManager::Get().FindConsoleVariable(TEXT("r.FidelityFX.FI.Enabled"));
	if(FSRFrameGenEnable)
	{
		FSRFrameGenEnable->Set(FSRMode.bFrameGenEnabled ? 1 : 0, ECVF_SetByConsole);
		UE_LOG(LogUpscalerUtils, Display,
			TEXT("%s - Executed r.FidelityFX.FI.Enabled %d"),
			ANSI_TO_TCHAR(__FUNCTION__), FSRMode.bFrameGenEnabled);
	}

	const int QualityMode = static_cast<int>(FSRMode.Quality);
	static IConsoleVariable* FSRQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.FidelityFX.FSR3.QualityMode"));
	if(FSRQuality)
	{
		FSRQuality->Set(QualityMode);
		UE_LOG(LogUpscalerUtils, Display,
			TEXT("%s - Executed r.FidelityFX.FSR3.QualityMode %d"),
			ANSI_TO_TCHAR(__FUNCTION__), QualityMode);
	}

	if(bSaveConfig)
	{
		UE_LOG(LogTemp, Display, TEXT("%s - Saving..."), ANSI_TO_TCHAR(__FUNCTION__));
		if(UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings())
		{
			Settings->SetCurrentUpscaler(EUpscaler::FSR);
			Settings->SetFSRQualityMode(FSRMode.Quality);
			Settings->SetFSRFrameGenEnabled(FSRMode.bFrameGenEnabled);
			Settings->SaveSettings();
			UE_LOG(LogTemp, Display, TEXT("%s - Save completed!"), ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}
		UE_LOG(LogTemp, Display, TEXT("%s - Save failed!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void UUpscalerSwitcherUtils::DisableUpscaling(bool bSaveConfig)
{
	const FString CommandReset = "r.ScreenPercentage.Reset";
	static IConsoleVariable* ScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	
	// disable fsr upscaling
	GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FSR3.Enabled 0"));
	// disable fsr frame gen
	GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FI.Enabled 0"));

	// disable dlss
	//UDLSSLibrary::EnableDLSS(false);
	EnableDLSS(false);
	
	//GEngine->Exec(GEngine->GetWorld(), *CommandReset);

	if(ScreenPercentage)
	{
		UE_LOG(LogTemp, Display, TEXT("%s - ScreenPercentage reset to 100 with IConsoleVariable"), ANSI_TO_TCHAR(__FUNCTION__));
		ScreenPercentage->Set(100.0f);
	}
	
	
	if(bSaveConfig)
	{
		UE_LOG(LogTemp, Display, TEXT("%s - Saving..."), ANSI_TO_TCHAR(__FUNCTION__));
		if(UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings())
		{
			Settings->SetCurrentUpscaler(EUpscaler::None);
			Settings->SaveSettings();
			UE_LOG(LogTemp, Display, TEXT("%s - Save completed!"), ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}
		UE_LOG(LogTemp, Display, TEXT("%s - Save failed!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}


void UUpscalerSwitcherUtils::ApplySavedUpscaler()
{
	UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
	if(!Settings) {
		UE_LOG(LogUpscalerUtils, Warning,
			TEXT("%s - Failed because defalut GameUserSettings is not set to UUpscalerGameUserSettings."),
			ANSI_TO_TCHAR(__FUNCTION__));
		return;
    }
	switch (Settings->GetCurrentUpscaler())
	{
	default:
	case EUpscaler::None:
		{
			DisableUpscaling();
		}
		break;
	case EUpscaler::FSR:
		{
			ApplyFSR(FFSRModeInformation(Settings->GetFSRQualityMode(), Settings->GetFSRFrameGenEnabled()));
		}
		break;
	case EUpscaler::DLSS:
		{
			ApplyDLSS(FDLSSModeInformation(Settings->GetDLSSQualityMode(),
				Settings->GetDLSSOptimalScreenPercentage(), Settings->GetDLSSFrameGenEnabled()));
		}
		break;
	}
	
}

EUpscaler UUpscalerSwitcherUtils::GetSavedUpscaler()
{
	const UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
	if(!Settings)
		return EUpscaler::None;
	return Settings->GetCurrentUpscaler();
}

void UUpscalerSwitcherUtils::SetDLSSQuality(UDLSSMode_Custom Quality, float OptimalScreenPercentage, bool bSaveConfig, bool bApplyUpscaling)
{
	UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
	if(!Settings)
	{
		UE_LOG(LogUpscalerUtils, Warning,
			TEXT("%s - Failed because defalut GameUserSettings is not set to UUpscalerGameUserSettings."),
			ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	Settings->SetDLSSQualityMode(Quality);
	Settings->SetDLSSOptimalScreenPercentage(OptimalScreenPercentage);
	
	if(bSaveConfig)
	{
		Settings->SaveSettings();
	}

	if(bApplyUpscaling && IsDLSSEnabled())
	{
		ApplySavedUpscaler();
	}
}

void UUpscalerSwitcherUtils::SetFSRQuality(EFFXFSR3QualityMode_Custom Quality, bool bSaveConfig, bool bApplyUpscaling)
{
	UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
	if(!Settings)
	{
		UE_LOG(LogUpscalerUtils, Warning,
			TEXT("%s - Failed because defalut GameUserSettings is not set to UUpscalerGameUserSettings."),
			ANSI_TO_TCHAR(__FUNCTION__));
		return;
	};

	Settings->SetFSRQualityMode(Quality);
	if(bSaveConfig)
	{
		Settings->SaveSettings();
	}

	if(bApplyUpscaling && GetFSREnabled())
	{
		ApplySavedUpscaler();
	}
}

void UUpscalerSwitcherUtils::SetFSRFramegen(bool bFrameGenEnabled, bool bSaveConfig, bool bApplyUpscaling)
{
	UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
	if(!Settings)
	{
		UE_LOG(LogUpscalerUtils, Warning,
			TEXT("%s - Failed because defalut GameUserSettings is not set to UUpscalerGameUserSettings."),
			ANSI_TO_TCHAR(__FUNCTION__));
		return;
	};

	Settings->SetFSRFrameGenEnabled(bFrameGenEnabled);
	if(bSaveConfig)
	{
		Settings->SaveSettings();
	}

	if(bApplyUpscaling)
	{
		ApplySavedUpscaler();
	}
}

bool UUpscalerSwitcherUtils::GetFSREnabled()
{
	// Find the console variable
	if (const IConsoleVariable* FSR3EnabledCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.FidelityFX.FSR3.Enabled")))
	{
		// Get the value (0 or 1)
		const int32 FSR3EnabledValue = FSR3EnabledCVar->GetInt();
		return FSR3EnabledValue == 1;
	}
	UE_LOG(LogUpscalerUtils, Warning,
			TEXT("%s - FSR plugin is not enabled. Returning false!"),
			ANSI_TO_TCHAR(__FUNCTION__));
	return false;
}

bool UUpscalerSwitcherUtils::GetDLSSEnabled()
{
	return IsDLSSEnabled();
}



void UUpscalerSwitcherUtils::EnableDLSS(bool bEnabled)
{
#if WITH_DLSS
	static IConsoleVariable* CVarDLSSEnable = IConsoleManager::Get().FindConsoleVariable(TEXT("r.NGX.DLSS.Enable"));
	if (CVarDLSSEnable)
	{
		CVarDLSSEnable->Set(bEnabled ? 1 : 0, ECVF_SetByCommandline);

		// Spatial upscalers such as NIS might set this to 0, but we need r.TemporalAA.Upscaler to be 1 for DLSS to work.
		// but we don't want to change the Cvar if DLSS is not active as to avoid impacting other code paths
		// we don't need to set r.TemporalAA.Upsampling since r.TemporalAA.Upscaler implies that
		if (bEnabled)
		{
			static const auto CVarTemporalAAUpscaler = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TemporalAA.Upscaler"));
			CVarTemporalAAUpscaler->Set(1, ECVF_SetByCommandline);
			// restore denoising state in case it was disabled before
			EnableDLSSRR(bDenoisingRequested);
		}
		else if (IsDLSSRREnabled())
		{
			// Denoising currently only runs as an upscaler pass, so if DLSS-SR is disabled, we automatically disable DLSS-RR as well
			EnableDLSSRR(false);
			bDenoisingRequested = true;
			UE_LOG(LogUpscalerUtils, Warning, TEXT("DLSS denoising unsupported without DLSS super resolution, disabled denoising"));
		}
	}
#endif
}

void UUpscalerSwitcherUtils::EnableDLSSRR(bool bEnabled)
{
#if WITH_DLSS
	
	bDenoisingRequested = bEnabled;
	static IConsoleVariable* CVarDLSSRREnable = IConsoleManager::Get().FindConsoleVariable(TEXT("r.NGX.DLSS.DenoiserMode"));
	const bool bDLSSRREnabled = CVarDLSSRREnable->GetInt() != 0;
	if (bDLSSRREnabled == bEnabled)
	{
		return;
	}

	// Denoising currently only runs as an upscaler pass, so only enable DLSS-RR if DLSS-SR is enabled
	if (bEnabled && IsDLSSEnabled())
	{
		CVarDLSSRREnable->Set(1);
		PreviousShadowDenoiser       = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.Denoiser"))->GetInt();
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.Denoiser"))->Set(0);
		PreviousLumenSSR             = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.ScreenSpaceReconstruction"))->GetInt();
		PreviousLumenTemporal        = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.Temporal"))->GetInt();
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.ScreenSpaceReconstruction"))->Set(0);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.Temporal"))->Set(0);
		static const auto CVarTemporalAAUpscaler = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TemporalAA.Upscaler"));
		CVarTemporalAAUpscaler->Set(1, ECVF_SetByCommandline);
		int LumenBilateralFilter = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.BilateralFilter"))->GetInt();
		if (LumenBilateralFilter != 0)
		{
			// engine bug in 5.2 and 5.3 prevents us from changing the value of this cvar from Blueprints
			UE_LOG(LogUpscalerUtils, Warning, TEXT("r.Lumen.Reflections.BilateralFilter should be disabled when DLSS Ray Reconstruction is enabled"));
		}
	}
	else if (!bEnabled)
	{
		CVarDLSSRREnable->Set(0);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.Denoiser"))->Set(PreviousShadowDenoiser);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.ScreenSpaceReconstruction"))->Set(PreviousLumenSSR);
		IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.Reflections.Temporal"))->Set(PreviousLumenTemporal);
	}
#endif
}

bool UUpscalerSwitcherUtils::IsDLSSRREnabled()
{
#if WITH_DLSS
	static const IConsoleVariable* CVarDLSSDenoiserMode = IConsoleManager::Get().FindConsoleVariable(TEXT("r.NGX.DLSS.DenoiserMode"));
	const bool bDLSSRREnabled = CVarDLSSDenoiserMode && (CVarDLSSDenoiserMode->GetInt() != 0);
	return bDLSSRREnabled;
#else
	return false;
#endif
}
bool UUpscalerSwitcherUtils::IsDLSSEnabled()
{
#if WITH_DLSS
	static const IConsoleVariable* CVarDLSSEnable = IConsoleManager::Get().FindConsoleVariable(TEXT("r.NGX.DLSS.Enable"));
	const bool bDLSSEnabled = CVarDLSSEnable && (CVarDLSSEnable->GetInt() != 0);
	return bDLSSEnabled;
#else
	return false;
#endif
}