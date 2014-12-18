#include "StdAfx.h"
#include "TimeUtils.h"

#include < time.h >
#include <windows.h> //I've ommited this line.
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  116444736000000000Ui64 // CORRECT
#else
#define DELTA_EPOCH_IN_MICROSECS  116444736000000000ULL // CORRECT
#endif


TimeUtils::TimeUtils(void)
{
}


TimeUtils::~TimeUtils(void)
{
}

ULONGLONG TimeUtils::getSystemMilliSecond()
{
	FILETIME ft;
	unsigned __int64 tmpres = 0;
	static int tzflag;

	GetSystemTimeAsFileTime(&ft);

	tmpres |= ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;

	/*converting file time to unix epoch*/
	tmpres -= DELTA_EPOCH_IN_MICROSECS; 
	tmpres /= 10000;  /*convert into milliseconds*/
	return tmpres;
}

int TimeUtils::getTimeFormat(struct tm* pTimeInfo)
{
	return getTimeFormat(pTimeInfo->tm_year + 1900, pTimeInfo->tm_mon + 1, pTimeInfo->tm_mday);
}

int TimeUtils::getTimeFormat(int year, int month, int day)
{
	return (10000 * year) + (100 * month) + day;
}

bool TimeUtils::isSameDay(time_t time1, time_t time2)
{
	struct tm* pInfo1 = localtime(&time1);
	struct tm* pInfo2 = localtime(&time2);
	return pInfo1->tm_year == pInfo2->tm_year && pInfo1->tm_mon == pInfo2->tm_mon && pInfo1->tm_mday == pInfo2->tm_mday;
}