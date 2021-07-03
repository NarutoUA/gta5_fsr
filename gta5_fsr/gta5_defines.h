#pragma once

inline constexpr auto GTA5_CB_SLOT_MAIN_GLOBALS = 2u;
inline constexpr auto GTA5_CB_SLOT_POSTFX = 5u;
inline constexpr auto GTA5_CB_SLOT_POSTFX_FXAA = 6u;

struct gta5_main_globals_cb_t
{
    std::array<float, 64> _pad;
    int msaa;
};
