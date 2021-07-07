#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define FSR_COMMON_LOG_PREFIX "GTA5_FSR: "
#define FSR_COMMON_API_D3D11

#include <filesystem>
#include <array>
#include <memory>
#include <optional>
#include <windows.h>
#include <d3d11.h>

#include <wil/resource.h>
#include <wil/com.h>

#include "gta5_defines.h"
#include "helpers/helpers.h"
#include "wrappers/wrappers.h"

#include "CSuperResolutionMgr.h"
