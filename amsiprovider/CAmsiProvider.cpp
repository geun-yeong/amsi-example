////////////////////////////////////////////////////
// 
// CAmsiProvider.cpp of amsiprovider
// 
////////////////////////////////////////////////////

#include <stdio.h>

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
	LPSTR content_hexstr = MakeHexString(content_addr, min(content_size, 16));

	//
	// 일부 .Net 프로그램이 AMSI를 이용하는 경우
	// 자신의 PE 헤더를 AMSI Scan 함수로 전달하기 때문에
	// 문자열 출력이 아닌 Hex String을 출력토록 작성
	//
	OutputDebugFormatStringA(
		"[amsiprovider] AMSI data\n"
		"App Name: %ws\n"
		"Content Name: %ws\n"
		"Content Size: %llu\n"
		"Content: %s",
		(app_name && wcslen(app_name)) ? app_name : L"<empty>", 
		(content_name && wcslen(content_name)) ? content_name : L"<empty>",
		content_size, 
		content_hexstr
	);

	delete[] app_name, content_name, content_hexstr;
	
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

LPSTR
CAmsiProvider::MakeHexString(
	_In_ PBYTE bin, 
	_In_ size_t len
)
{
	size_t str_len = len * 3 + 1;
	char* str = new char[str_len]; // null char
	ZeroMemory(str, str_len);

	char* offset = str;
	for (size_t i = 0; i < len; i++) {
		sprintf_s(offset, str_len, "%02X ", bin[i]);
		offset += 3;
		str_len -= 3;
	}

	return str;
}