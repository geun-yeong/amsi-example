////////////////////////////////////////////////////
// 
// CAmsiClient.cpp of amsiclient
// 
// This file has 
//   - Constructor and Destructor of CAmsiClient class
//   - Scan methods
// 
////////////////////////////////////////////////////

#include "CAmsiClient.h"

#include <stdio.h>
#include <exception>

const wchar_t* AMSI_CLIENT_NAME = L"AmsiClient";

CAmsiClient::CAmsiClient() 
{
	HRESULT result = AmsiInitialize(AMSI_CLIENT_NAME, &_amsi_context);
	if (result != S_OK) {
		char except_msg[64];
		sprintf_s(except_msg, 64, "Could not initialize AMSI context (HRESULT = %u)", result);
		throw std::exception(except_msg);
	}
}

CAmsiClient::~CAmsiClient() 
{
	// When the app is finished with the AMSI API
	// it must call AmsiUninitialize
	AmsiUninitialize(_amsi_context);
	_amsi_context = NULL;
}

AMSI_RESULT
CAmsiClient::Scan(
	void *data, 
	size_t data_size
)
{
	HRESULT result = S_OK;
	AMSI_RESULT amsi_result = AMSI_RESULT_NOT_DETECTED;

	result = AmsiScanBuffer(
		_amsi_context,		// amsi context
		data,				// buffer
		data_size,			// length
		AMSI_CLIENT_NAME,	// content name(filename, URL, unique script ID or etc)
		nullptr,			// amsi session
		&amsi_result		// result
	);

	if (result != S_OK) {
		return AMSI_RESULT_NOT_DETECTED;
	}

	return amsi_result;
}

AMSI_RESULT
CAmsiClient::Scan(
	wchar_t* text
)
{
	HRESULT result = S_OK;
	AMSI_RESULT amsi_result = AMSI_RESULT_NOT_DETECTED;

	result = AmsiScanString(
		_amsi_context,		// amsi context
		text,				// string
		AMSI_CLIENT_NAME,	// content name(filename, URL, unique script ID or etc)
		nullptr,			// amsi session
		&amsi_result		// result
	);

	if (result != S_OK) {
		return AMSI_RESULT_NOT_DETECTED;
	}

	return amsi_result;
}