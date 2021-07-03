#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <filesystem>
#include <array>
#include <memory>
#include <optional>
#include <windows.h>
#include <d3d11.h>

#include <wil/resource.h>
#include <wil/com.h>

#include "gta5_defines.h"
#include "helpers/debug_helper.h"
#include "wrappers/wrappers.h"

#include "CSuperResolutionMgr.h"
