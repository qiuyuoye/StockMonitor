#pragma once

#include "Analyse.h"
#include "stock/Stock.h"

#include <vector>

enum PV
{
	E_PV_NONE,
	E_PV_Peak,
	E_PV_Valley,
};

class __declspec(dllexport) PVItem
{
public:
	PVItem();
	~PVItem();

	PV m_pv;
	double m_value;
	unsigned int m_index;
};

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

	Period m_period;
	double m_max;
	double m_min;
	float m_rate;
	unsigned int m_startIndex;
	unsigned int m_endIndex;
};

typedef std::vector<PVItem> PVItemVec;
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
	void getPVs(const std::vector<double>& datas, PVItemVec& items);

	void getDividePVs(const std::vector<double>& datas, PVItemVec& items);

	void getFluctuatePVs(PVItemVec& items);

	void getPeriods(const PVItemVec& pvItems, PeriodItemVec& prdItems);

};

