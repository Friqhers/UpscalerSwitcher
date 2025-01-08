# UpscalerSwitcher Plugin for Unreal Engine 5.3

The `UpscalerSwitcher` module is a plugin for Unreal Engine that allows dynamic switching between different upscaling methods such as FSR and DLSS.

## Features

- Enable/Disable FSR and DLSS upscaling.
- Switch between different upscaling methods at runtime.
- Adjust screen percentage and quality settings for each upscaling method.

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

## Usage

### Enabling/Disabling Upscaling

You can enable or disable upscaling methods using the `UUpscalerSwitcherUtils` class:

```cpp
#include "UpscalerSwitcherUtils.h"

// Enable FSR (FidelityFX Super Resolution) for upscaling
// If another upscaler (e.g., DLSS) is currently enabled, it will be disabled before enabling FSR
UUpscalerSwitcherUtils::SetUpscalerMethod(EUpscaler::FSR, true);

// Enable DLSS for upscaling
// This will first disable any previously enabled upscaling method (e.g., FSR) and then enable DLSS
UUpscalerSwitcherUtils::SetUpscalerMethod(EUpscaler::DLSS, true);

// Disable all upscaling methods (FSR, DLSS, etc.), reverting to native resolution rendering
UUpscalerSwitcherUtils::SetUpscalerMethod(EUpscaler::None, true);
```
### Adjusting Quality Settings

Adjust the quality settings for FSR and DLSS using the UUpscalerGameUserSettings class:
```cpp
#include "UpscalerGameUserSettings.h"
#include "UpscalerSwitcherUtils.h"

// Set FSR quality mode
UUpscalerGameUserSettings* Settings = UUpscalerGameUserSettings::GetUpscalerGameUserSettings();
Settings->SetFSRQualityMode(EFSRQualityMode::Quality);

// Set DLSS quality mode
Settings->SetDLSSQualityMode(EDLSSQualityMode::Performance);

// Apply the quality mode
UUpscalerSwitcherUtils::ApplyCurrentUpscalerMethod();
```

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
    UUpscalerSwitcherUtils::ApplyCurrentUpscalerMethod();
}
```
## License
This project is licensed under the CC BY-NC License.
