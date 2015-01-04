// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"


#include <iostream>
#include <fstream>

static const int kMaxLogLen = 16*1024;

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

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
