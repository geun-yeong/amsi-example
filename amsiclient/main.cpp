////////////////////////////////////////////////////
// 
// main.cpp of amsiclient
// 
////////////////////////////////////////////////////

#include <Windows.h>
#include <stdio.h>
#include <amsi.h>
#pragma comment(lib, "amsi.lib")

#include "CAmsiClient.h"

int wmain(
	int argc, 
	wchar_t *argv[]
) 
{
	if (argc != 2) {
		printf("Usage: %ws <SCANNED_TEXT>\n", argv[0]);
		return ERROR_INVALID_PARAMETER;
	}

	CAmsiClient amsi; 
	AMSI_RESULT result = amsi.Scan(argv[1]);

	if(AmsiResultIsMalware(result)) {
		printf("MALICIOUS\n");
	}
	else if (AmsiResultIsBlockedByAdmin(result)) {
		printf("BLOCKED BY ADMIN\n");
	}
	else {
		printf("NORMAL\n");
	}

	return ERROR_SUCCESS;
}