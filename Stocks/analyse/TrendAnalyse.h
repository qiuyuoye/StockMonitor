#pragma once

#include "Analyse.h"
#include "stock/Stock.h"

#include <vector>

enum Period
{
	E_PRD_NONE,
	E_PRD_UP,
	E_PRD_DOWN,
	E_PRD_SHOCK,
};

class __declspec(dllexport) PeriodItem
{
public:
	PeriodItem();
	~PeriodItem();

	void compute(const std::vector<double>& datas);

	void computeParams(const std::vector<double>& datas);

	void computeTrend(const std::vector<double>& datas);

	int getDays() const {	return m_endIndex - m_startIndex + 1;};

	Period m_period;

	unsigned int m_startIndex;
	unsigned int m_endIndex;

	ULONGLONG m_avg;
	ULONGLONG m_max;
	ULONGLONG m_min;

	double m_avgSlope;
	double m_slopSD;	   //Standard Deviation
};

typedef std::vector<PeriodItem> PeriodItemVec;

class __declspec(dllexport) TrendAnalyse : public Analyse
{
public:
	TrendAnalyse(void);
	~TrendAnalyse(void);

protected:
	virtual void onAnalyse(const Stock* pStock);

	virtual void printResult(const Stock* pStock);

private:

	void analysisPeriods(const std::vector<double>& datas, PeriodItemVec& prdItems);

	void initPeriods(const std::vector<double>& datas, PeriodItemVec& prdItems);

	PeriodItemVec mergePeriods(const std::vector<double>& datas, const PeriodItemVec& prdItems, int day);

};

