#pragma once

inline constexpr auto GTA5_CB_SLOT_MAIN_GLOBALS = 2u;
inline constexpr auto GTA5_CB_SLOT_POSTFX = 5u;
inline constexpr auto GTA5_CB_SLOT_POSTFX_FXAA = 6u;

inline constexpr auto GTA5_MIN_FRAME_SCALE = 0.5f;

struct gta5_main_globals_cb_t
{
    std::array<float, 64> _pad;
    int msaa;
};
