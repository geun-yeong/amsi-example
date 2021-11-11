////////////////////////////////////////////////////
// 
// CAmsiProvider.h of amsiprovider
// 
////////////////////////////////////////////////////

#pragma once

#include <Windows.h>
#include <amsi.h>
#include <wrl/module.h>

class
	DECLSPEC_UUID("00000000-0000-0000-0000-000000000000")
	CAmsiProvider : public Microsoft::WRL::RuntimeClass<
								Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
								IAntimalwareProvider,
								Microsoft::WRL::FtmBase
	>
{
public:
	IFACEMETHOD(Scan)(_In_ IAmsiStream* stream, _Out_ AMSI_RESULT* amsi_result) override;
	IFACEMETHOD_(VOID, CloseSession)(_In_ ULONGLONG session) override;
	IFACEMETHOD(DisplayName)(_Outptr_ LPWSTR* display_name) override;

private:
	LPSTR MakeHexString(_In_ PBYTE bin, _In_ size_t len);
};