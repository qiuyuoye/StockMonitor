#pragma once

#include "Analyse.h"
#include "DataAnalyse.h"
#include "TrendAnalyse.h"


class TrendPredictAnalyse : public Analyse
{
public:
	explicit TrendPredictAnalyse(StockCPtr pStock);
	~TrendPredictAnalyse(void);

	virtual void analyse();

	virtual void print();

private:
	DataAnalyse m_dataAnalyse;
	TrendAnalyse m_trendAnalyse;

};

