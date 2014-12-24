#include "StdAfx.h"
#include "TrendAnalyse.h"
#include <algorithm>

using namespace std;

PeriodItem::PeriodItem()
	: m_period(E_PRD_NONE)
	, m_max(0)
	, m_min(0)
	, m_avg(0)
	, m_startIndex(0)
	, m_endIndex(0)
	, m_avgSlope(0.0f)
	, m_slopSD(0.0f)
{

}

PeriodItem::~PeriodItem()
{

}

void PeriodItem::compute(const std::vector<double>& datas)
{
	computeParams(datas);
	computeTrend(datas);
}

void PeriodItem::computeParams(const std::vector<double>& datas)
{
	ULONGLONG total = 0;
	m_max = 0;
	m_min = 99999999999;
	for(unsigned int i = m_startIndex; i <= m_endIndex; ++i)
	{
		m_max = (ULONGLONG)(datas[i] > m_max ? datas[i] : m_max);
		m_min = (ULONGLONG)(datas[i] < m_min ? datas[i] : m_min);
		total += (ULONGLONG)(datas[i]);

		if(i != m_endIndex)
		{
			m_avgSlope += (datas[i + 1] - datas[i]);
		}
	}
	m_avg = total / getDays();
	m_avgSlope /= (m_endIndex - m_startIndex);

	for(unsigned int i = m_startIndex; i < m_endIndex; ++i)
	{
		double x = (datas[i + 1] - datas[i]);
		m_slopSD = m_slopSD + (x - m_avgSlope) * (x - m_avgSlope);
	}
	m_slopSD = sqrtf(m_slopSD / (m_endIndex - m_startIndex));
}

void PeriodItem::computeTrend(const std::vector<double>& datas)
{
	for(unsigned int i = m_startIndex; i <= m_endIndex; ++i)
	{

	}
}


TrendAnalyse::TrendAnalyse(void)
{
}


TrendAnalyse::~TrendAnalyse(void)
{
}

void TrendAnalyse::onAnalyse( const Stock* pStock )
{
	vector<double> adjCloseVec;
	pStock->getAllAdjustClose(adjCloseVec);
	//getPVs(adjCloseVec, itemVec);

	PeriodItemVec prdItemVec;
	analysisPeriods(adjCloseVec, prdItemVec);

}

void TrendAnalyse::printResult( const Stock* pStock )
{

}

void TrendAnalyse::analysisPeriods( const std::vector<double>& datas, PeriodItemVec& prdItems )
{
	initPeriods(datas, prdItems);
	PeriodItemVec mergedPeriods1 = mergePeriods(datas, prdItems, 3);
	PeriodItemVec mergedPeriods2 = mergePeriods(datas, prdItems, 4);
	PeriodItemVec mergedPeriods3 = mergePeriods(datas, prdItems, 5);
	int j = 0;
}

void TrendAnalyse::initPeriods(const std::vector<double>& datas, PeriodItemVec& prdItems)
{
	std::vector<double>::size_type i = 0;
	while(i < datas.size())
	{
		if(i + 1 == datas.size())
		{
			PeriodItem item;
			item.m_startIndex = i;
			item.m_endIndex = i;
			item.m_period = (datas[i] > datas[i - 1] ? E_PRD_UP : E_PRD_DOWN);
			prdItems.push_back(item);

			break;
		}

		std::vector<double>::size_type index = i + 1;
		PeriodItem item;
		item.m_period = (datas[index] > datas[i] ? E_PRD_UP : E_PRD_DOWN);
		item.m_startIndex = i;
		item.m_endIndex = index;

		while(index < datas.size() && index + 1 < datas.size())
		{
			if((item.m_period == E_PRD_UP && datas[index] < datas[index + 1])
				|| (item.m_period == E_PRD_DOWN && datas[index] >= datas[index + 1]))
			{

				item.m_endIndex = index + 1;
				++index;
			}
			else
			{
				break;
			}
		}

		prdItems.push_back(item);

		i = item.m_endIndex + 1;
		
	}

	for(PeriodItemVec::iterator iter = prdItems.begin(); iter != prdItems.end(); ++iter)
	{
		iter->compute(datas);
	}
}

PeriodItemVec TrendAnalyse::mergePeriods(const std::vector<double>& datas, const PeriodItemVec& prdItems, int day)
{
	if(prdItems.size() == 1)
	{
		return prdItems;
	}
	PeriodItemVec tmpPrdItems = prdItems;
	PeriodItemVec resultVec;
	
	for(PeriodItemVec::iterator iter = tmpPrdItems.begin(); iter != tmpPrdItems.end(); ++iter)
	{
		if(iter->getDays() > day)
		{
			resultVec.push_back(*iter);
			continue;
		}

		PeriodItemVec::iterator iterNext = iter + 1;
		PeriodItemVec::reverse_iterator iterPre = resultVec.rbegin();
		bool frontMerge = false;
		if(iterPre == resultVec.rend())
		{
			frontMerge = false;
		}
		else if(iterNext == tmpPrdItems.end())
		{
			frontMerge = true;
		}
		else
		{
			double r1 = iter->m_avgSlope - iterPre->m_avgSlope;
			double r2 = iter->m_avgSlope - iterNext->m_avgSlope;
			if(r1 < r2)
			{
				frontMerge = true;
			}
			else
			{
				frontMerge = false;
			}
		}

		if(frontMerge)
		{
			iterPre->m_endIndex = iter->m_endIndex;
			iterPre->compute(datas);
		}
		else
		{
			iterNext->m_startIndex = iter->m_startIndex;
			iterNext->compute(datas);
		}

	}

	return resultVec;
}