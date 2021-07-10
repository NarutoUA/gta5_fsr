#include "stdafx.h"
#include <VersionHelpers.h>

wil::unique_hmodule g_hOrigDll;
wil::unique_hmodule g_hProxyDll;
bool g_bPrintDebug = true;
bool g_bFiveM = false;
bool g_bWrapFiveMSwapChain = false;

gta5_fsr_cfg_t g_Cfg;
d3d11_exports_t g_D3D11Exports;

void DetectFiveM()
{
    char path[MAX_PATH] = { 0 };
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    g_bFiveM = strstr(path, "FiveM") != nullptr;
}

void ReadConfig(gta5_fsr_cfg_t& cfg)
{
    CSimpleIniA ini;
    if (ini.LoadFile(GTA5_FSR_CFG_FILENAME) == SI_OK)
    {
        ini_read_bool(ini, "DEBUG", GTA5_FSR_CFG_KEY_PRINT_DEBUG, g_bPrintDebug);

        print_debug("GTA5_FSR: Reading config file...");

        ini_read_bool(ini, "GLOBAL", GTA5_FSR_CFG_USE_FSR, cfg.UseFidelityFxSuperResolution);
        ini_read_bool(ini, "GLOBAL", GTA5_FSR_CFG_AUTO_SHARPNESS, cfg.AutoSharpness);

        float sharpness;
        if (ini_read_numeric(ini, "GLOBAL", GTA5_FSR_CFG_SHARPNESS, sharpness) && sharpness >= 0.f && sharpness <= 1.f)
            cfg.Sharpness = sharpness;

        int code;
        if (ini_read_numeric(ini, "INPUT", GTA5_FSR_CFG_KEY_TOGGLE_FSR, code))
            cfg.KeyToggleFSR = code;

        if (ini_read_numeric(ini, "INPUT", GTA5_FSR_CFG_KEY_UPDATE_SHARPNESS, code))
            cfg.KeyUpdateSharpness = code;

        ini_read_bool(ini, "PROXY", GTA5_FSR_CFG_KEY_ENABLE_PROXY, cfg.EnableProxyLibrary);
        if (cfg.EnableProxyLibrary)
        {
            ini_read_bool(ini, "PROXY", GTA5_FSR_CFG_KEY_INIT_PROXY, cfg.InitProxyFunctions);
            ini_read_string(ini, "PROXY", GTA5_FSR_CFG_KEY_PROXY_LIB, cfg.ProxyLibrary);
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ReadConfig(g_Cfg);
        DetectFiveM();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void LoadWrappedModule()
{
    static std::once_flag flag;
    std::call_once(flag, []() 
    {
        bool bLoadOrigLib = true;
        bool bUseOrigExports = true;
        if (g_Cfg.EnableProxyLibrary && g_Cfg.ProxyLibrary.empty() == false)
        {
            g_hProxyDll.reset(LoadLibrary(g_Cfg.ProxyLibrary.c_str()));
            if (g_hProxyDll.is_valid())
            {
                bLoadOrigLib = false;
                if (g_Cfg.InitProxyFunctions && ReadD3D11Exports(g_hProxyDll.get(), g_D3D11Exports))
                {
                    bUseOrigExports = false;
                }
            }
            else
            {
                print_debug("GTA5_FSR: Error on loading proxy dll: %d", GetLastError());
            }
        }
        if (bLoadOrigLib || bUseOrigExports)
        {
            TCHAR buf[MAX_PATH];
            GetSystemDirectory(buf, MAX_PATH);
            g_hOrigDll.reset(LoadLibrary((std::filesystem::path(buf) / "d3d11.dll").string().c_str()));

            if (bUseOrigExports)
                ReadD3D11Exports(g_hOrigDll.get(), g_D3D11Exports);

        }
        print_debug("GTA5_FSR: Loaded wrapped module. LoadOrigLib: %d, UseOrigExports: %d", bLoadOrigLib, bUseOrigExports);
    });
}

__declspec(dllexport) HRESULT WINAPI _D3D11CreateDevice(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _COM_Outptr_opt_ ID3D11Device** ppDevice,
    _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext)
{
    LoadWrappedModule();
    if (g_D3D11Exports.D3D11CreateDevice == nullptr)
    {
        print_debug("GTA5_FSR: D3D11CreateDevice is nullptr");
        TerminateProcess(GetCurrentProcess(), 0);
        return E_FAIL;
    }

    print_debug("GTA5_FSR: D3D11CreateDevice");
    auto result = g_D3D11Exports.D3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
    if (g_bFiveM && IsWindows10OrGreater() && ppDevice && *ppDevice && ppImmediateContext && *ppImmediateContext)
    {
        g_bWrapFiveMSwapChain = true;

        CSuperResolutionMgr::Initialize(*ppDevice, *ppImmediateContext);

        print_debug("GTA5_FSR: Wrapping ID3D11Device [d3d11-fivem]");
        *ppDevice = new ID3D11DeviceWrapper(*ppDevice);

        print_debug("GTA5_FSR: Wrapping ID3D11DeviceContext [d3d11-fivem]");
        *ppImmediateContext = new ID3D11DeviceContextWrapper(*ppImmediateContext);
    }

    return result;
}

__declspec(dllexport) HRESULT WINAPI _D3D11CreateDeviceAndSwapChain(
    _In_opt_ IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    _In_opt_ CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    _COM_Outptr_opt_ IDXGISwapChain** ppSwapChain,
    _COM_Outptr_opt_ ID3D11Device** ppDevice,
    _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
    _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext)
{
    LoadWrappedModule();
    if (g_D3D11Exports.D3D11CreateDeviceAndSwapChain == nullptr)
    {
        print_debug("GTA5_FSR: D3D11CreateDeviceAndSwapChain is nullptr");
        TerminateProcess(GetCurrentProcess(), 0);
        return E_FAIL;
    }

    print_debug("GTA5_FSR: D3D11CreateDeviceAndSwapChain");
    auto result = g_D3D11Exports.D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

    if (ppDevice && *ppDevice && ppImmediateContext && *ppImmediateContext)
    {
        CSuperResolutionMgr::Initialize(*ppDevice, *ppImmediateContext);

        print_debug("GTA5_FSR: Wrapping ID3D11Device [d3d11]");
        *ppDevice = new ID3D11DeviceWrapper(*ppDevice);

        print_debug("GTA5_FSR: Wrapping ID3D11DeviceContext [d3d11]");
        *ppImmediateContext = new ID3D11DeviceContextWrapper(*ppImmediateContext);
    }

    if (ppSwapChain && *ppSwapChain)
    {
        print_debug("GTA5_FSR: Wrapping IDXGISwapChain [d3d11]");
        *ppSwapChain = new IDXGISwapChainWrapper(*ppSwapChain);
    }

    return result;
}
