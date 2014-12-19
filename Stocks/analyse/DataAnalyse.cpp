#include "StdAfx.h"
#include "DataAnalyse.h"


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

DataAnalyse::DataAnalyse(void)
	: m_maxVol(0.0)
	, m_minVol(99999999999999.0)
	, m_avgVol(0.0)
{
}


DataAnalyse::~DataAnalyse(void)
{
}

void DataAnalyse::onAnalyse(const Stock* pStock)
{
	const ItemVec& items = pStock->getItems();
	if(items.empty())
	{
		return;
	}
	
	for(ItemVec::const_iterator iter = items.begin(); iter != items.end(); ++iter)
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

void DataAnalyse::printResult(const Stock* pStock)
{
	Log("----DataAnalyse----");
	Log("Stock: %s", pStock->getSymbol().c_str());
	Log("Average Volume: %.0f", m_avgVol);
	Log("Max Volume: %.0f", m_maxVol);
	Log("Min Volume: %.0f", m_minVol);
	Log("-------------------");
}