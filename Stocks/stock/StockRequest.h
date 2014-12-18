#pragma once

#include <string>

class __declspec(dllexport) StockRequest
{
public:
	StockRequest(void);
	~StockRequest(void);

	static std::string requestHistory(const std::string& stockSymbol, int startYear, int startMon, int startDay, int endYear, int endMon, int endDay);

	void requestCurrentInfo();

};

