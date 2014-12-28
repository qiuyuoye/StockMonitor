#pragma once

#include "Analyse.h"
#include "stock/Stock.h"

#include <vector>

enum Period
{
	E_PRD_NONE,
	E_PRD_UP,
	E_PRD_DOWN,
};

class __declspec(dllexport) PeriodItem
{
public:
	static double getSlopDiff(const PeriodItem& item1, const PeriodItem& item2);

	static double getSDDiff(const PeriodItem& item1, const PeriodItem& item2);

	static double getFluctuationDiff(const PeriodItem& item1, const PeriodItem& item2);

	PeriodItem();
	explicit PeriodItem(float ratio);
	~PeriodItem();

	void initResult();

	void compute(const std::vector<double>& datas);

	void computeParams(const std::vector<double>& datas);

	void computeTrend(const std::vector<double>& datas);

	void print(const Stock& stocks, int offset);

	int getDays() const {	return m_endIndex - m_startIndex + 1;};

	Period m_period;

	unsigned int m_startIndex;
	unsigned int m_endIndex;

	double m_avg;
	double m_max;
	double m_min;

	double m_avgSlope;
	double m_slopSD;	   //Standard Deviation

	double m_fluctuation;

	double m_maxGain;
	double m_maxLoss;

private:
	float m_ratio;
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

	void mergePeriods(const std::vector<double>& datas, unsigned int size, PeriodItemVec& prdItems);

	void mergeSimilarPeriods(const std::vector<double>& datas, PeriodItemVec& prdItems, double minSlopDiff, double minFluDiff);

	void removeInvalides(PeriodItemVec& prdItems);

	double getAvgSlops(const PeriodItemVec& prdItems);

	double getAvgStandardDeviation(const PeriodItemVec& prdItems);

};

