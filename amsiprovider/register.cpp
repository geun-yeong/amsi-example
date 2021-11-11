////////////////////////////////////////////////////
// 
// register.cpp of amsiprovider
// 
// Code from https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/AmsiProvider/AmsiProvider.cpp
// 
////////////////////////////////////////////////////

#include <strsafe.h>

#include "CAmsiProvider.h"
#include "register.h"

using namespace Microsoft::WRL;

#pragma region COM server boilerplate

STDAPI DllCanUnloadNow(void) {
	return Module<InProc>::GetModule().Terminate() ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(
	_In_ REFCLSID ref_clsid,
	_In_ REFIID ref_iid,
	_Outptr_ LPVOID FAR* cls_obj
)
{
	return Module<InProc>::GetModule().GetClassObject(ref_clsid, ref_iid, cls_obj);
}

#pragma endregion

CoCreatableClass(CAmsiProvider);

#pragma region Install / uninstall

HRESULT SetKeyStringValue(_In_ HKEY key, _In_opt_ PCWSTR subkey, _In_opt_ PCWSTR value, _In_ PCWSTR data)
{
    LONG status = RegSetKeyValue(key, subkey, value, REG_SZ, data, (wcslen(data) + 1) * sizeof(wchar_t));
    return HRESULT_FROM_WIN32(status);
}

STDAPI DllRegisterServer()
{
    wchar_t module_path[MAX_PATH];
    if (GetModuleFileName(g_current_module, module_path, ARRAYSIZE(module_path)) >= ARRAYSIZE(module_path))
    {
        return E_UNEXPECTED;
    }

    wchar_t clsid[40];
    if (StringFromGUID2(__uuidof(CAmsiProvider), clsid, ARRAYSIZE(clsid)) == 0)
    {
        return E_UNEXPECTED;
    }

    //
    // 레지스트리에 AMSI 구성 정보 등록
    //
    wchar_t registry_subkey[200];
    HRESULT hr = StringCchPrintf(registry_subkey, ARRAYSIZE(registry_subkey), L"Software\\Classes\\CLSID\\%ls", clsid);
    if (FAILED(hr)) return hr;

    hr = SetKeyStringValue(HKEY_LOCAL_MACHINE, registry_subkey, nullptr, L"SampleAmsiProvider");
    if (FAILED(hr)) return hr;

    hr = StringCchPrintf(registry_subkey, ARRAYSIZE(registry_subkey), L"Software\\Classes\\CLSID\\%ls\\InProcServer32", clsid);
    if (FAILED(hr)) return hr;

    hr = SetKeyStringValue(HKEY_LOCAL_MACHINE, registry_subkey, nullptr, module_path);
    if (FAILED(hr)) return hr;

    hr = SetKeyStringValue(HKEY_LOCAL_MACHINE, registry_subkey, L"ThreadingModel", L"Both");
    if (FAILED(hr)) return hr;

    //
    // 레지스트리에 AMSI 프로필 정보 등록
    //
    hr = StringCchPrintf(registry_subkey, ARRAYSIZE(registry_subkey), L"Software\\Microsoft\\AMSI\\Providers\\%ls", clsid);
    if (FAILED(hr)) return hr;

    hr = SetKeyStringValue(HKEY_LOCAL_MACHINE, registry_subkey, nullptr, L"AMSI Provider Example");
    if (FAILED(hr)) return hr;

    return S_OK;
}

STDAPI DllUnregisterServer()
{
    wchar_t clsid[40];
    if (StringFromGUID2(__uuidof(CAmsiProvider), clsid, ARRAYSIZE(clsid)) == 0)
    {
        return E_UNEXPECTED;
    }

    wchar_t registry_subkey[200];
    HRESULT hr = StringCchPrintf(registry_subkey, ARRAYSIZE(registry_subkey), L"Software\\Microsoft\\AMSI\\Providers\\%ls", clsid);
    if (FAILED(hr)) return hr;

    LONG status = RegDeleteTree(HKEY_LOCAL_MACHINE, registry_subkey);
    if (status != NO_ERROR && status != ERROR_PATH_NOT_FOUND) return HRESULT_FROM_WIN32(status);

    hr = StringCchPrintf(registry_subkey, ARRAYSIZE(registry_subkey), L"Software\\Classes\\CLSID\\%ls", clsid);
    if (FAILED(hr)) return hr;

    status = RegDeleteTree(HKEY_LOCAL_MACHINE, registry_subkey);
    if (status != NO_ERROR && status != ERROR_PATH_NOT_FOUND) return HRESULT_FROM_WIN32(status);

    return S_OK;
}

#pragma endregion