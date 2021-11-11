////////////////////////////////////////////////////
// 
// dllmain.cpp of amsiprovider
// 
////////////////////////////////////////////////////

#include <Windows.h>
#include <wrl/module.h>
using namespace Microsoft::WRL;

#include "log.h"

HMODULE g_current_module;

BOOL 
APIENTRY 
DllMain(
    HMODULE module,
    DWORD  reason_for_call,
    LPVOID reserved
)
{
    WCHAR current_module_name[MAX_PATH]{ 0, };

    switch (reason_for_call) {
    case DLL_PROCESS_ATTACH:
        OutputDebugStringA("[amsiprovider] AMSIProvider DLL was loaded");

        DisableThreadLibraryCalls(module);

        GetModuleFileName(NULL, current_module_name, ARRAYSIZE(current_module_name));
        OutputDebugFormatStringA("[amsiprovider] Current module is %ws", current_module_name);
        
        Module<InProc>::GetModule().Create();

        g_current_module = module;

        break;

    case DLL_PROCESS_DETACH:
        OutputDebugStringA("[amsiprovider] AMSIProvider DLL will be unloaded");

        Module<InProc>::GetModule().Terminate();

        g_current_module = NULL;

        break;
    }

    return TRUE;
}