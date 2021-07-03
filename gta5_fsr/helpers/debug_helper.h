#pragma once

extern bool g_bPrintDebug;

template <std::size_t N, class ...Args>
void print_debug(const char(&format)[N], Args... args)
{
    // this is still unsafe but who cares
    static_assert(sizeof...(args) == 0 || N < 128, "Format string is too long");

    if (g_bPrintDebug == false)
        return;

    if (sizeof...(args))
    {
        char buf[256] = { 0 };
        sprintf_s(buf, format, std::forward<Args>(args)...);
        OutputDebugStringA(buf);
    }
    else
    {
        OutputDebugStringA(format);
    }
}

template <std::size_t N, class ...Args>
bool print_debug_timeout(ULONGLONG& tick, const ULONGLONG timeout, const char(&format)[N], Args... args)
{
    if (g_bPrintDebug == false)
        return false;

    if ((GetTickCount64() - tick) > timeout)
    {
        tick = GetTickCount64();
        print_debug(format, std::forward<Args>(args)...);
        return true;
    }
    return false;
}
