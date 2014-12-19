// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

void Log(const char * pszFormat, ...);

#define LOG(format, ...)      Log(format, ##__VA_ARGS__)
#define USE_DLL     __declspec(dllexport)
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
