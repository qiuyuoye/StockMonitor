#include "StdAfx.h"
#include "DataAnalyse.h"
#include "utils/StringUtils.h"


DataAnalyse::DataAnalyse(StockCPtr stockPtr, IndexType indexType)
	: Analyse(stockPtr)
	, m_maxVol(0.0)
	, m_minVol(99999999999999.0)
	, m_avgVol(0.0)
	, m_indexType(indexType)
{
}


DataAnalyse::~DataAnalyse(void)
{
}

void DataAnalyse::analyse()
{
	std::vector<double> datas;
	m_pStock->getDatasByIndex(datas, m_indexType);

	if(datas.empty())
	{
		return;
	}

	for(UINT i = 0; i < datas.size(); ++i)
	{
		if(datas[i] < 1)
		{
			continue;
		}

		m_avgVol += datas[i];
		m_maxVol = (m_maxVol >= datas[i] ? m_maxVol : datas[i]);
		m_minVol = (m_minVol <= datas[i] ? m_minVol : datas[i]);
	}

	m_avgVol = m_avgVol / datas.size();

}

void DataAnalyse::print()
{
	Log("--------------   DataAnalyse : %s  ----------------", m_pStock->getSymbol().c_str());
	Log("Average: %s, Max: %s, Min: %s", 
		StringUtils::formatWithCommas(m_avgVol).c_str(),
		StringUtils::formatWithCommas(m_maxVol).c_str(),
		StringUtils::formatWithCommas(m_minVol).c_str());
}