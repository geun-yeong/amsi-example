#include <Windows.h>
#include <stdio.h>
#include "log.h"

#if defined(_DEBUG) && _DEBUG

void OutputDebugFormatStringA(
	const char* format,
	...
) 
{
	va_list va_ptr;
	va_start(va_ptr, format);

	int len = _vscprintf(format, va_ptr) + 1;
	char* msg = new char[len];
	vsprintf_s(msg, len, format, va_ptr);
	OutputDebugStringA(msg);
	delete[] msg;

	va_end(va_ptr);
}

#else

void OutputDebugFormatStringA(
	const char* format,
	...
)
{
	return;
}

#endif