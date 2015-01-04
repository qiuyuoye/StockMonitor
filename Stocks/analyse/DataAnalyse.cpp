#include "StdAfx.h"
#include "DataAnalyse.h"
#include "utils/StringUtils.h"


DataResult::DataResult()
	: m_avgOpenChange(0.0f)
	, m_avgOpenRateChange(0.0f)
	, m_avgCloseChange(0.0f)
	, m_avgCloseRateChange(0.0f)
	, m_avgAdjustCloseChange(0.0f)
	, m_avgAdjustCloseRateChange(0.0f)
	, m_avgHighChange(0.0f)
	, m_avgHighRateChange(0.0f)
	, m_avgLowChange(0.0f)
	, m_avgLowRateChange(0.0f)
	, m_maxPrice(0.0f)
	, m_minPrice(0.0f)
	, m_avgVolChange(0.0)
	, m_avgVolRateChange(0.0)
{

}

DataResult::~DataResult()
{

}

DataAnalyse::DataAnalyse(StockCPtr stockPtr)
	: Analyse(stockPtr)
	, m_maxVol(0.0)
	, m_minVol(99999999999999.0)
	, m_avgVol(0.0)
{
}


DataAnalyse::~DataAnalyse(void)
{
}

void DataAnalyse::onAnalyse()
{
	const ItemVec& items = m_pStock->getItems();
	if(items.empty())
	{
		return;
	}

	for(auto iter = items.begin(); iter != items.end(); ++iter)
	{
		if(iter->volume < 1)
		{
			continue;
		}

		m_avgVol += iter->volume;
		m_maxVol = (m_maxVol >= iter->volume ? m_maxVol : iter->volume);
		m_minVol = (m_minVol <= iter->volume ? m_minVol : iter->volume);
	}

	m_avgVol = m_avgVol / items.size();

}

void DataAnalyse::printResult()
{
	Log("----   DataAnalyse : %s  ----", m_pStock->getSymbol().c_str());
	Log("Average Volume: %s", StringUtils::volumeToString(m_pStock->getSymbol(), m_avgVol).c_str());
	Log("Max Volume: %s", StringUtils::volumeToString(m_pStock->getSymbol(), m_maxVol).c_str());
	Log("Min Volume: %s", StringUtils::volumeToString(m_pStock->getSymbol(), m_minVol).c_str());
	Log("-------------------");
}