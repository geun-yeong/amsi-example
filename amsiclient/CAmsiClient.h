////////////////////////////////////////////////////
// 
// CAmsiClient.h of amsiclient
// 
// This file has 
//   - CAmsiClient class
// 
////////////////////////////////////////////////////

#pragma once

#include <amsi.h>
#pragma comment(lib, "amsi.lib")

extern const wchar_t *AMSI_CLIENT_NAME;

class CAmsiClient {
private:
	HAMSICONTEXT _amsi_context;

public:
	CAmsiClient();
	~CAmsiClient();

	AMSI_RESULT Scan(void *data, size_t data_size);
	AMSI_RESULT Scan(wchar_t *text);
};