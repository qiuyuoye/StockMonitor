#pragma once


class TimeUtils
{
public:
	static ULONGLONG getSystemMilliSecond();

	static int getTimeFormat(struct tm* pTimeInfo);

	static int getTimeFormat(int year, int month, int day);

	static bool isSameDay(time_t time1, time_t time2);

private:
	TimeUtils(void);
	~TimeUtils(void);
};

