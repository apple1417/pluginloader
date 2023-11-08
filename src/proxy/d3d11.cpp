#include "pch.h"

#include "util.h"

// Based on https://github.com/FromDarkHell/BL3DX11Injection/

namespace pluginloader::proxy {

namespace {

HMODULE dx11_dll_handle = nullptr;

FARPROC d3d11_core_create_device_ptr = nullptr;
FARPROC d3d11_create_device_ptr = nullptr;
FARPROC d3d11_create_device_and_swap_chain_ptr = nullptr;

// NOLINTBEGIN(readability-identifier-naming, readability-identifier-length)

DLL_EXPORT HRESULT D3D11CoreCreateDevice(void* fact,
                                         void* adapt,
                                         unsigned int flag,
                                         void* fl,
                                         unsigned int featureLevels,
                                         void** ppDev) {
    return reinterpret_cast<decltype(&D3D11CoreCreateDevice)>(d3d11_core_create_device_ptr)(
        fact, adapt, flag, fl, featureLevels, ppDev);
}

DLL_EXPORT HRESULT D3D11CreateDevice(void* adapt,
                                     unsigned int dt,
                                     void* soft,
                                     unsigned int flags,
                                     int* ft,
                                     unsigned int fl,
                                     unsigned int ver,
                                     void** ppDevice,
                                     void* featureLevel,
                                     void** context) {
    return reinterpret_cast<decltype(&D3D11CreateDevice)>(d3d11_create_device_ptr)(
        adapt, dt, soft, flags, ft, fl, ver, ppDevice, featureLevel, context);
}

DLL_EXPORT HRESULT D3D11CreateDeviceAndSwapChain(void* adapt,
                                                 unsigned int dt,
                                                 void* soft,
                                                 unsigned int flags,
                                                 int* ft,
                                                 unsigned int fl,
                                                 unsigned int ver,
                                                 void* swapChainDesc,
                                                 void** swapChain,
                                                 void** ppDevice,
                                                 void* featureLevel,
                                                 void** context) {
    return reinterpret_cast<decltype(&D3D11CreateDeviceAndSwapChain)>(
        d3d11_create_device_and_swap_chain_ptr)(adapt, dt, soft, flags, ft, fl, ver, swapChainDesc,
                                                swapChain, ppDevice, featureLevel, context);
}
// NOLINTEND(readability-identifier-naming, readability-identifier-length)

}  // namespace

void init(HMODULE /*this_dll*/) {
    // Suspend all other threads to prevent a giant race condition
    const util::ThreadSuspender suspender{};

    wchar_t buf[MAX_PATH];
    if (GetSystemDirectoryW(&buf[0], ARRAYSIZE(buf)) == 0) {
        std::cerr << "Unable to find system dll directory! We're probably about to crash.\n";
        return;
    }

    auto system_dx11 = std::filesystem::path{static_cast<wchar_t*>(buf)} / "d3d11.dll";
    dx11_dll_handle = LoadLibraryA(system_dx11.generic_string().c_str());

    d3d11_core_create_device_ptr = GetProcAddress(dx11_dll_handle, "D3D11CoreCreateDevice");
    d3d11_create_device_ptr = GetProcAddress(dx11_dll_handle, "D3D11CreateDevice");
    d3d11_create_device_and_swap_chain_ptr =
        GetProcAddress(dx11_dll_handle, "D3D11CreateDeviceAndSwapChain");
}

void free(void) {
    if (dx11_dll_handle != nullptr) {
        FreeLibrary(dx11_dll_handle);
        dx11_dll_handle = nullptr;
    }
}

}  // namespace pluginloader::proxy
