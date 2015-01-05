#pragma once

#include "Analyse.h"
#include "stock/Stock.h"

#include <vector>
#include <set>
#include <map>

enum Trend
{
	E_TRD_NONE,
	E_TRD_UP,
	E_TRD_FLUCTUATION_UP,
	E_TRD_DOWN,
	E_TRD_FLUCTUATION_DOWN,
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

	void print(StockCPtr pStock, int offset);

	int getDays() const {	return m_endIndex - m_startIndex + 1;};

	std::string getPeriod();

	Trend m_period;

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

	explicit TrendAnalyse(StockCPtr stockPtr, IndexType majorIndex);
	~TrendAnalyse(void);

	virtual void analyse();

	virtual void print();

	void pushRelative(IndexType relIdx)
	{
		m_relIdxVec.push_back(relIdx);
	}

	const PeriodItemVec& getMajor()
	{
		return m_majorVec;
	}

	const PeriodItemVec& getRelative(IndexType relIdx)
	{
		return m_relPrdsMap[relIdx];
	}

private:
	class DiffItem
	{
	public:
		unsigned int m_index;
		double m_value;
	};

	typedef std::map<IndexType, PeriodItemVec> PrdItemVecMap;

	void analysisMajor();

	void initMajorPrds(const std::vector<double>& datas);

	void mergePeriods(const std::vector<double>& datas, unsigned int size);

	void mergeSimilarPeriods(const std::vector<double>& datas);

	double getAvgSlops(const PeriodItemVec& prdItems);

	double getAvgStandardDeviation(const PeriodItemVec& prdItems);

	void analyseRelPeriods();
	
	IndexType m_majorIdx;
	std::vector<IndexType> m_relIdxVec;

	PeriodItemVec m_majorVec;
	PrdItemVecMap m_relPrdsMap;

	int m_mergedSize;
};

