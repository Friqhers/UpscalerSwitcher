#include "UpscalerSwitcherUtils.h"
#include "DLSSLibrary.h"
#include "UpscalerGameUserSettings.h"

void UUpscalerSwitcherUtils::SetUpscalerMethod(const EUpscaler Upscaler, bool SaveConfig)
{
	UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscalerMethod -> first!"));
	UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
	if(!Settings) return;
	
	const FString CommandReset = "r.ScreenPercentage.Reset";
	static IConsoleVariable* ScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));

	if(SaveConfig)
	{
		Settings->SetCurrentUpscaler(Upscaler);
		Settings->SaveSettings();
	}
	
	switch (Upscaler)
	{
	default:
	case EUpscaler::None:
		{
			// disable fsr upscaling
			GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FSR3.Enabled 0"));
			// disable fsr frame gen
			GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FI.Enabled 0"));

			// disable dlss
			UDLSSLibrary::EnableDLSS(false);


		
			//GEngine->Exec(GEngine->GetWorld(), *CommandReset);

			if(ScreenPercentage)
			{
				UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscaler -> ScreenPercentage reset to 100 with IConsoleVariable"));
				ScreenPercentage->Set(100.0f);
			}
			
			// const FString CommandDisable = "r.ScreenPercentage " + FString::FromInt(100);
			// GEngine->Exec(GEngine->GetWorld(), *CommandDisable);
			// UE_LOG(LogTemp, Warning, TEXT("ULYDGameUserSettings::ApplyUpscaler -> Applied disable upscaler command: %s"), *CommandDisable);
		}
		break;
	case EUpscaler::FSR:
		{
			// disable dlss
			UDLSSLibrary::EnableDLSS(false);

			// enable fsr upscaling
			const FString FSRCommand = "r.FidelityFX.FSR3.Enabled 1";
			GEngine->Exec(GEngine->GetWorld(), *FSRCommand);
			UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscaler -> Applied FSR command: %s"), *FSRCommand);
			
			const FString FrameGenCommand = "r.FidelityFX.FI.Enabled " + FString::FromInt(Settings->GetFSRFrameGenEnabled() ? 1 : 0);
			// enable fsr frame gen
			GEngine->Exec(GEngine->GetWorld(), *FrameGenCommand);
			UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscaler -> Applied FSR command: %s"), *FrameGenCommand);

			const int QualityMode = static_cast<int>(Settings->GetFSRQualityMode());
			const FString FSRQCommand = "r.FidelityFX.FSR3.QualityMode " + FString::FromInt(QualityMode);
			GEngine->Exec(GEngine->GetWorld(), *FSRQCommand);
			UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscaler -> Applied FSR command: %s"), *FSRQCommand);
			// r.FidelityFX.FSR3.QualityMode
		}
		
		break;
	case EUpscaler::DLSS:
		{
			//UFFXFSR3Settings::bEnabled = 0;
			// disable fsr upscaling	
			GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FSR3.Enabled 0"));
			// disable fsr frame gen
			GEngine->Exec(GEngine->GetWorld(), *FString("r.FidelityFX.FI.Enabled 0"));

			if(!UDLSSLibrary::IsDLSSSupported())
			{
				UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscaler -> Can't apply DLSS since it is not supported!"));
				//@TODO: maybe popup widget for unsupported text warning?
				Settings->SetCurrentUpscaler(EUpscaler::None);
				ApplyCurrentUpscalerMethod();
				return;
			}

			// enable dlss
			UDLSSLibrary::EnableDLSS(true);

			bool bIsSupported, bIsFixedScreenPercentage;
			float OptimalScreenPercentage, MinScreenPercentage, MaxScreenPercentage, OptimalSharpness;

			// apply dlss quality
			UDLSSLibrary::GetDLSSModeInformation(
				Settings->GetDLSSQualityMode(),
				Settings->GetScreenResolution(),
				bIsSupported,OptimalScreenPercentage,
				bIsFixedScreenPercentage,MinScreenPercentage,
				MaxScreenPercentage, OptimalSharpness
				);
			
			//GEngine->Exec(GEngine->GetWorld(), *CommandReset);


			if(ScreenPercentage)
			{
				UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscaler -> ScreenPercentage reset to %s with IConsoleVariable"), *FString::SanitizeFloat(OptimalScreenPercentage));
				ScreenPercentage->Set(OptimalScreenPercentage);
			}
			
			// const FString Command = "r.ScreenPercentage " + FString::SanitizeFloat(OptimalScreenPercentage);
			// GEngine->Exec(GEngine->GetWorld(), *Command);
			// UE_LOG(LogTemp, Warning, TEXT("UUpscalerSwitcherUtils::ApplyUpscaler -> Applied DLSS command: %s"), *Command);
		
		}
		
		break;
	}
}

void UUpscalerSwitcherUtils::ApplyCurrentUpscalerMethod()
{
	const UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
	if(!Settings) return;
	SetUpscalerMethod(Settings->GetCurrentUpscaler(), false);
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
	return false;
}
