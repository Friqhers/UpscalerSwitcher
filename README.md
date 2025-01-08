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

// Enable FSR
UUpscalerSwitcherUtils::SetUpscalerMethod(EUpscaler::FSR, true);

// Disable FSR
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

## License
This project is licensed under the CC BY-NC License.
