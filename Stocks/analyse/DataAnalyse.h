#pragma once

#include "Analyse.h"
#include "stock/Stock.h"

class __declspec(dllexport) DataResult
{
public:
	DataResult();
	~DataResult();

	float m_avgOpenChange;
	float m_avgOpenRateChange;

	float m_avgCloseChange;
	float m_avgCloseRateChange;

	float m_avgAdjustCloseChange;
	float m_avgAdjustCloseRateChange;

	float m_avgHighChange;
	float m_avgHighRateChange;

	float m_avgLowChange;
	float m_avgLowRateChange;

	float m_maxPrice;
	float m_minPrice;

	double m_avgVolChange;
	double m_avgVolRateChange;
};

typedef std::vector<DataResult> DataResultVec;

class __declspec(dllexport) DataAnalyse : public Analyse
{
public:

	explicit DataAnalyse(StockCPtr stockPtr);
	~DataAnalyse(void);

protected:
	virtual void onAnalyse();

	virtual void printResult();

private:

	DataResultVec m_results;

	double m_maxVol;

	double m_minVol;

	double m_avgVol;
};

