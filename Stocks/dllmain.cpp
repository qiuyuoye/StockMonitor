// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"


#include <iostream>
#include <fstream>

static const int kMaxLogLen = 16*1024;

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

void Log(const char * pszFormat, ...)
{
	char szBuf[kMaxLogLen];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, kMaxLogLen, kMaxLogLen, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[kMaxLogLen] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");

	WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), nullptr, FALSE);
	printf("%s\n", szBuf);

	std::ofstream file("log.txt", std::ios::app);
	file <<szBuf << "\n";
	file.close();
}
