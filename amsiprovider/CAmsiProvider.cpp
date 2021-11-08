////////////////////////////////////////////////////
// 
// CAmsiProvider.cpp of amsiprovider
// 
////////////////////////////////////////////////////

#include "CAmsiProvider.h"
#include "log.h"

wchar_t* 
GetAttributeStringValue(
	_In_ IAmsiStream* stream,
	_In_ AMSI_ATTRIBUTE attr
)
{
	ULONG alloc_size = 0;
	
	HRESULT result = stream->GetAttribute(attr, 0, nullptr, &alloc_size);
	if (result != E_NOT_SUFFICIENT_BUFFER) {
		return nullptr;
	}

	unsigned char* value = new unsigned char[alloc_size];
	result = stream->GetAttribute(attr, alloc_size, value, &alloc_size);
	if (!SUCCEEDED(result)) {
		delete[] value;
		return nullptr;
	}

	return (wchar_t*)value;
}

template<typename T>
T
GetAttributeValue(
	_In_ IAmsiStream* stream,
	_In_ AMSI_ATTRIBUTE attr
)
{
	T ret;

	ULONG actual_size;
	HRESULT result = stream->GetAttribute(attr, sizeof(T), reinterpret_cast<PBYTE>(&ret), &actual_size);
	if (SUCCEEDED(result) && actual_size == sizeof(T)) {
		return ret;
	}

	return T();
}

HRESULT
CAmsiProvider::Scan(
	_In_ IAmsiStream* stream, 
	_Out_ AMSI_RESULT* amsi_result
)
{
	LPCWSTR app_name = GetAttributeStringValue(stream, AMSI_ATTRIBUTE_APP_NAME);
	LPCWSTR content_name = GetAttributeStringValue(stream, AMSI_ATTRIBUTE_CONTENT_NAME);
	ULONGLONG content_size = GetAttributeValue<ULONGLONG>(stream, AMSI_ATTRIBUTE_CONTENT_SIZE);
	PBYTE content_addr = GetAttributeValue<PBYTE>(stream, AMSI_ATTRIBUTE_CONTENT_ADDRESS);

	LPWSTR line_end_addr = wcschr((wchar_t*)content_addr, L'\n');
	if (!line_end_addr) {
		line_end_addr = wcschr((wchar_t*)content_addr, L'\0');
	}
	size_t line_len = line_end_addr - (LPWSTR)content_addr;
	LPWSTR contents = new WCHAR[line_len + 1];
	ZeroMemory(contents, sizeof(WCHAR) * (line_len + 1));

	wcsncpy_s(contents, line_len + 1, (const wchar_t*)content_addr, line_len);

	OutputDebugFormatStringA(
		"[amsiprovider] AMSI data\n"
		"App Name: %ws\n"
		"Content Name: %ws\n"
		"Content Size: %llu\n"
		"Content: %ws",
		(app_name && wcslen(app_name)) ? app_name : L"<empty>", 
		(content_name && wcslen(content_name)) ? content_name : L"<empty>",
		content_size, contents
	);

	delete[] app_name, content_name, contents;
	
	* amsi_result = AMSI_RESULT_CLEAN;

	return S_OK;
}

VOID
CAmsiProvider::CloseSession(
	_In_ ULONGLONG session
)
{
	OutputDebugStringA("[amsiprovider] Session was closed");
}

HRESULT 
CAmsiProvider::DisplayName(
	_Outptr_ LPWSTR* display_name
)
{
	*display_name = const_cast<LPWSTR>(L"AMSI Provider Example");
	return S_OK;
}