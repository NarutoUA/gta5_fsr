#pragma once

#include <optional>
#include <string>

inline constexpr auto GTA5_FSR_CFG_FILENAME = "gta5_fsr.ini";

// [GLOBAL]
inline constexpr auto GTA5_FSR_CFG_USE_FSR = "UseFidelityFxSuperResolution";
inline constexpr auto GTA5_FSR_CFG_AUTO_SHARPNESS = "AutoSharpness";
inline constexpr auto GTA5_FSR_CFG_SHARPNESS = "Sharpness";

// [INPUT]
inline constexpr auto GTA5_FSR_CFG_KEY_TOGGLE_FSR = "KeyToggleFSR";
inline constexpr auto GTA5_FSR_CFG_KEY_UPDATE_SHARPNESS = "KeyUpdateSharpness";

// [DEBUG]
inline constexpr auto GTA5_FSR_CFG_KEY_PRINT_DEBUG = "PrintDebug";

// [PROXY]
inline constexpr auto GTA5_FSR_CFG_KEY_ENABLE_PROXY = "EnableProxyLibrary";
inline constexpr auto GTA5_FSR_CFG_KEY_INIT_PROXY = "InitProxyFunctions";
inline constexpr auto GTA5_FSR_CFG_KEY_PROXY_LIB = "ProxyLibrary";

struct gta5_fsr_cfg_t
{
    // [GLOBAL]
    bool UseFidelityFxSuperResolution = true;
    bool AutoSharpness = true;
    float Sharpness = 0.5f;

    // [INPUT]
    std::optional<int> KeyToggleFSR;
    std::optional<int> KeyUpdateSharpness;

    // [DEBUG]
    // bool PrintDebug = true; // should be global var because of common code

    // [PROXY]
    bool EnableProxyLibrary = false;
    bool InitProxyFunctions = false;
    std::string ProxyLibrary;
};
