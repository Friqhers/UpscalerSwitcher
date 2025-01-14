# UpscalerSwitcher Plugin for Unreal Engine

The `UpscalerSwitcher` is a plugin for Unreal Engine that allows dynamic switching between different upscaling methods such as FSR and DLSS.

## Features

- Enable/Disable FSR and DLSS upscaling.
- Switch between different upscaling methods at runtime.
- Adjust quality settings for each upscaling method.

## Installation

1. Clone or download the `UpscalerSwitcher` plugin into the `Plugins` directory of your Unreal Engine project.
2. Add the following line to your project's `.uproject` file to include the plugin:

    ```json
    "Plugins": [
        {
            "Name": "UpscalerSwitcher",
            "Enabled": true
        }
    ]
    ```

3. Rebuild your project to include the plugin.
4. Set your GameUserSettings class to UpscalerGameUserSettings in Project Settings->Engine-General Settings->GameUserSettingsClass.
5. Download DLSS and FSR plugins from their respective site and include them to your project.

## Usage

### Enabling/Disabling Upscaling

You can enable or disable upscaling methods using the `UUpscalerSwitcherUtils` class:

```cpp
#include "UpscalerSwitcherUtils.h"

// If another upscaler (e.g., DLSS) is currently enabled, it will be disabled before enabling FSR
// Enable saved FSR mode 
UUpscalerSwitcherUtils::ApplyFSR(FFSRModeInformation(Settings->GetFSRQualityMode(),
                                 Settings->GetFSRFrameGenEnabled()));

// This will first disable any previously enabled upscaling method (e.g., FSR) and then enable DLSS
// Enable saved DLSS mode
UUpscalerSwitcherUtils::ApplyDLSS(FDLSSModeInformation(Settings->GetDLSSQualityMode(),
                                    Settings->GetDLSSOptimalScreenPercentage(), Settings->GetDLSSFrameGenEnabled()));

// Disable all upscaling methods (FSR, DLSS, etc.), reverting to native resolution rendering
UUpscalerSwitcherUtils::DisableUpscaling();
```
### Adjusting Quality Settings

#### Adjusting Quality Setting for DLSS

Example c++ code to adjust the quality setting for the DLSS:
```cpp
#include "UpscalerGameUserSettings.h"
#include "UpscalerSwitcherUtils.h"
#include "DLSSLibrary.h" // PS. Dont forget to add DLSSBlueprint module in to your project.Build.cs like PrivateDependencyModuleNames.AddRange(new string[] {"DLSSBlueprint"});
 
// Get settings class
UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();

// Set DLSS quality mode to performance
Settings->SetDLSSQualityMode(UDLSSMode_Custom::Performance);

// Get optimal screen percentage for the quality mode
bool bIsSupported, bIsFixedScreenPercentage;
float OptimalScreenPercentage, MinScreenPercentage, MaxScreenPercentage, OptimalSharpness;
UDLSSLibrary::GetDLSSModeInformation(
    Settings->GetDLSSQualityMode(),
    Settings->GetScreenResolution(),
    bIsSupported,OptimalScreenPercentage,
    bIsFixedScreenPercentage,MinScreenPercentage,
    MaxScreenPercentage, OptimalSharpness
    );

// Save the optimal screen percentage
Settings->SetDLSSOptimalScreenPercentage(OptimalScreenPercentage);

// Apply the current saved upscaler (DLSS) by Util function
UUpscalerSwitcherUtils::ApplySavedUpscaler();

// Also you can call the ApplySettings function of UpscalerGameUserSettings which will also call ApplySavedUpscaler
Settings->ApplySettings(false);
```

Example Blueprint usage:
![dlss_usage](https://github.com/user-attachments/assets/6590ab37-b056-4543-ba21-e2540e1a62a6)

#### Adjusting Quality Setting for FSR

Example C++ code to adjust the quality setting for the FSR:
```cpp
#include "UpscalerGameUserSettings.h"
#include "UpscalerSwitcherUtils.h"
#include "DLSSLibrary.h" // PS. Dont forget to add DLSSBlueprint module in to your project.Build.cs like PrivateDependencyModuleNames.AddRange(new string[] {"DLSSBlueprint"});
 
// Get settings class
UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();

// Set FSR quality mode to performance
Settings->SetFSRQualityMode(EFFXFSR3QualityMode_Custom::Performance);

// Set FSR frame gen enabled
Settings->SetFSRFrameGenEnabled(true);

// Apply the current saved upscaler (DLSS)
UUpscalerSwitcherUtils::ApplySavedUpscaler();
// Also you can call the ApplySettings function of UpscalerGameUserSettings which will also call ApplySavedUpscaler
Settings->ApplySettings(false);
```

Example Blueprint usage:
![fsr_usage](https://github.com/user-attachments/assets/3ec4227f-fd51-46fc-a744-6c4a14fa53da)



### Automatically Apply the Saved Upscaling Method on Game Start

To ensure that your game automatically applies the last saved upscaling method (e.g., DLSS, FSR, or native resolution) upon game startup, you should override the `Init` function in your custom `GameInstance` class. This ensures the saved upscaling preferences are applied seamlessly without requiring manual input from the player.

Follow these steps:

1. Open your `GameInstance` header file and define the `Init` function as follows:

```h
UCLASS()
class YOURPROJECT_API UYourGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Override the Init function to apply the current upscaling method on game start
    virtual void Init() override;
};
```

2. In the corresponding `GameInstance` source file, implement the `Init` function:

```cpp
void UYourGameInstance::Init()
{
    // Call the parent class's Init function to maintain existing initialization logic
    Super::Init();

    // Apply the saved upscaling method (e.g., DLSS, FSR, or None) from the last game session
    UUpscalerSwitcherUtils::ApplySavedUpscaler();
}
```
