#include "StdAfx.h"
#include "TrendPredictAnalyse.h"


TrendPredictAnalyse::TrendPredictAnalyse(StockCPtr pStock)
	: Analyse(pStock)
	, m_dataAnalyse(pStock, E_VOLUME)
	, m_trendAnalyse(pStock, E_ADJ_CLOSE)
{
	m_trendAnalyse.pushRelative(E_VOLUME);
}


TrendPredictAnalyse::~TrendPredictAnalyse(void)
{
}

void TrendPredictAnalyse::analyse()
{
	m_dataAnalyse.analyse();
	m_trendAnalyse.analyse();
}

void TrendPredictAnalyse::print()
{

}
