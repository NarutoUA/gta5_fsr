#include "stdafx.h"
#include <mutex>
#include <VersionHelpers.h>

wil::unique_hmodule g_hModule;
bool g_bPrintDebug = true;
bool g_bFiveM = false;
bool g_bWrapFiveMSwapChain = false;

void DetectFiveM()
{
    char path[MAX_PATH] = { 0 };
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    g_bFiveM = strstr(path, "FiveM") != nullptr;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DetectFiveM();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void LoadOrigModule()
{
    static std::once_flag flag;
    std::call_once(flag, []() 
    {
        TCHAR buf[MAX_PATH];
        GetSystemDirectory(buf, MAX_PATH);
        g_hModule.reset(::LoadLibrary((std::filesystem::path(buf) / "d3d11.dll").c_str()));
        
        print_debug("GTA5_FSR: Loaded original d3d11.dll");
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
    LoadOrigModule();
    auto proc = reinterpret_cast<decltype(_D3D11CreateDevice)*>(GetProcAddress(g_hModule.get(), "D3D11CreateDevice"));

    auto result = proc(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
    print_debug("GTA5_FSR: D3D11CreateDevice");

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
    LoadOrigModule();
    auto proc = reinterpret_cast<decltype(_D3D11CreateDeviceAndSwapChain)*>(GetProcAddress(g_hModule.get(), "D3D11CreateDeviceAndSwapChain"));
    print_debug("GTA5_FSR: D3D11CreateDeviceAndSwapChain");

    auto result = proc(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

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
