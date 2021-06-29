#pragma once
#include <array>

struct cb_easu_t
{
    std::array<float, 4> cViewportSize;
    std::array<float, 4> cTextureSize;
};

static_assert(sizeof(cb_easu_t) % 16 == 0, "Invalid cb_easu_t size");

struct cb_rcas_t
{
    float cSharpness;
private:
    std::array<std::byte, 12> _pad;
};

static_assert(sizeof(cb_rcas_t) % 16 == 0, "Invalid cb_rcas_t size");
