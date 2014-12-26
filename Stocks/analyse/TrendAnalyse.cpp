#include "StdAfx.h"
#include "TrendAnalyse.h"
#include <algorithm>
#include "utils/MathUtil.h"

using namespace std;


double PeriodItem::getSlopDiff(const PeriodItem& item1, const PeriodItem& item2)
{
	if((item1.m_avgSlope > 0.0 && item2.m_avgSlope < 0.0)
		|| (item1.m_avgSlope < 0.0 && item2.m_avgSlope > 0.0))
	{
		return abs(item1.m_avgSlope - item2.m_avgSlope) * 2.5;
	}
	return abs(item1.m_avgSlope - item2.m_avgSlope);
}

double PeriodItem::getSDDiff(const PeriodItem& item1, const PeriodItem& item2)
{
	return MathUtil::difference(item1.m_slopSD, item2.m_slopSD);
}

PeriodItem::PeriodItem(float ratio)
	: m_period(E_PRD_NONE)
	, m_ratio(ratio)
	, m_startIndex(0)
	, m_endIndex(0)
	, m_max(0)
	, m_min(0)
	, m_avg(0)
	, m_avgSlope(0.0f)
	, m_slopSD(0.0f)
	, m_maxGain(0.0f)
	, m_maxLoss(0.0f)
{

}

PeriodItem::PeriodItem()
	: m_period(E_PRD_NONE)
	, m_ratio(1.0f)
	, m_startIndex(0)
	, m_endIndex(0)
	, m_max(0)
	, m_min(0)
	, m_avg(0)
	, m_avgSlope(0.0f)
	, m_slopSD(0.0f)
	, m_maxGain(0.0f)
	, m_maxLoss(0.0f)
{

}

PeriodItem::~PeriodItem()
{

}

void PeriodItem::initResult()
{
	m_max = 0;
	m_min = 99999999999;
	m_avg = 0;
	m_avgSlope = 0;
	m_slopSD = 0;
	m_maxGain = 0;
	m_maxLoss = 0;
}

void PeriodItem::compute(const std::vector<double>& datas)
{
	initResult();
	computeParams(datas);
	computeTrend(datas);
}

void PeriodItem::computeParams(const std::vector<double>& datas)
{
	double total = 0.0;
	for(unsigned int i = m_startIndex; i <= m_endIndex; ++i)
	{
		m_max = (datas[i] > m_max ? datas[i] : m_max);
		m_min = (datas[i] < m_min ? datas[i] : m_min);
		total += (datas[i]);

		if(i != m_endIndex)
		{
			m_avgSlope = m_avgSlope + (datas[i + 1] - datas[i]) / m_ratio;
		}
	}
	m_avg = total / getDays();
	m_avgSlope /= (m_endIndex - m_startIndex);

	for(unsigned int i = m_startIndex; i < m_endIndex; ++i)
	{
		double x = (datas[i + 1] - datas[i]) / m_ratio;
		m_slopSD = m_slopSD + (x - m_avgSlope) * (x - m_avgSlope);
	}
	m_slopSD = sqrt(m_slopSD / (m_endIndex - m_startIndex));
}

void PeriodItem::computeTrend(const std::vector<double>& datas)
{
	if(m_avgSlope > 0.0f)
	{
		m_period = E_PRD_UP;
	}
	else
	{
		m_period = E_PRD_DOWN;
	}

	for(unsigned int i = m_startIndex; i < m_endIndex; ++i)
	{
		double x = (datas[i + 1] - datas[i]);

		m_slopSD = m_slopSD + (x - m_avgSlope) * (x - m_avgSlope);

		if(x > 0.0f)
		{
			m_maxGain += x;
		}
		else
		{
			m_maxLoss += x;
		}
	}
}

void PeriodItem::print(const Stock& stocks, int offset)
{
	const ItemVec& items = stocks.getItems();
	Log("%d-%d-%d   %d-%d-%d", items[m_startIndex + offset].year, items[m_startIndex + offset].month, items[m_startIndex + offset].day,
		items[m_endIndex + offset].year, items[m_endIndex + offset].month, items[m_endIndex + offset].day);
	Log("Trend %s", m_period == E_PRD_UP ? "Up" : "Down");
	Log("Max: %.4f, Min: %.4f, Avg: %.4f", m_max, m_min, m_avg);
	Log("Slop: %.4f, Standard Deviation: %.4f", m_avgSlope, m_slopSD);
	Log("Max Gain: %.4f, Max Loss %.4f", m_maxGain, m_maxLoss);
	
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
	pStock->getAllAdjustClose(adjCloseVec, 311);

	PeriodItemVec prdItemVec;
	analysisPeriods(adjCloseVec, prdItemVec);

	for(auto iter = prdItemVec.begin(); iter != prdItemVec.end(); ++iter)
	{
		iter->print(*pStock, 311);
	}

	int j = 0;
}

void TrendAnalyse::printResult( const Stock* pStock )
{

}

void TrendAnalyse::analysisPeriods( const std::vector<double>& datas, PeriodItemVec& prdItems )
{
	if(datas.size() <= 1)
	{
		Log("The size of datas must bigger than 1");
		return;
	}

	initPeriods(datas, prdItems);
	mergePeriods(datas, 20, prdItems);
}

void TrendAnalyse::initPeriods(const std::vector<double>& datas, PeriodItemVec& prdItems)
{
	double minData = 0;
	double maxData = 0;
	for(auto i = 0; i < datas.size(); ++i)
	{
		if(i == 0)
		{
			minData = datas[i];
			maxData = datas[i];
		}
		else 
		{
			if(minData > datas[i])
			{
				minData = datas[i];
			}

			if(maxData < datas[i])
			{
				maxData = datas[i];
			}
		}
	}

	float ratio = (float)(1.0 / (maxData - minData) * datas.size());

	std::vector<double>::size_type i = 0;
	while(i + 1 < datas.size())
	{
		std::vector<double>::size_type index = i + 1;
		PeriodItem item(ratio);
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

		i = item.m_endIndex;
		
	}

	for(auto iter = prdItems.begin(); iter != prdItems.end(); ++iter)
	{
		iter->compute(datas);
	}
}

void TrendAnalyse::mergePeriods(const std::vector<double>& datas, unsigned int size, PeriodItemVec& prdItems)
{

	if(prdItems.size() <= size)
	{
		return;
	}

	
	double a = 0.4f;
	while(prdItems.size() > size)
	{
		auto unmergeSize = prdItems.size();

		double avgSlop = getAvgSlops(prdItems);
		double avgSD = getAvgStandardDeviation(prdItems);

		mergeSimilarPeriods(datas, prdItems, avgSlop * a, avgSD * 1.2);
		
		unmergeSize == prdItems.size() ? a += 0.1f : a = 0.6f;

	}

	int j  =0 ;
}

void TrendAnalyse::mergeSimilarPeriods(const std::vector<double>& datas, PeriodItemVec& prdItems, double minSlopDiff, double minSDDiff)
{
	if(prdItems.empty())
	{
		return;
	}

	auto iterPre = prdItems.begin();
	auto iter = prdItems.begin() + 1;
	auto iterNext = iter + 1;

	while(iter != prdItems.end())
	{
		int merge = 0; //0: not merge  1: merge front 2: merge next

		double diffSlopPre = PeriodItem::getSlopDiff(*iterPre, *iter);
		double diffSDPre = PeriodItem::getSDDiff(*iterPre, *iter);
		
		if(diffSlopPre <= minSlopDiff && diffSDPre <= minSDDiff)
		{
			merge = 1;
		}

		if(iterNext != prdItems.end())
		{
			double diffSlopNext = PeriodItem::getSlopDiff(*iter, *iterNext);
			double diffSDNext = PeriodItem::getSDDiff(*iter, *iterNext);
			if((merge == 0 && diffSlopNext <= minSlopDiff && diffSDNext <= minSDDiff)
				|| (merge == 1 && diffSlopNext <= diffSlopPre && diffSDNext <= diffSDPre))
			{
				merge = 2;
			}
		}

		if(merge == 1)
		{
			iterPre->m_endIndex = iter->m_endIndex;
			iterPre->compute(datas);
			iter->m_period = E_PRD_NONE;

			++iter;
			++iterNext;
		}
		else if(merge == 2)
		{
			iter->m_endIndex = iterNext->m_endIndex;
			iter->compute(datas);
			iterNext->m_period = E_PRD_NONE;

			iterPre = iter;
			iter = iterNext;
			++iter;
			if(iter != prdItems.end())
			{
				iterNext = iter + 1;
			}
		}
		else
		{
			iterPre = iter;
			iter = iterNext;
			if(iterNext != prdItems.end())
			{
				++iterNext;
			}
			
		}
	}

	removeInvalides(prdItems);
}

void TrendAnalyse::removeInvalides(PeriodItemVec& prdItems)
{
	auto iterEnd = std::remove_if(prdItems.begin(), prdItems.end(), [](const PeriodItem& item){
		return item.m_period == E_PRD_NONE;
	});

	prdItems.resize(std::distance(prdItems.begin(), iterEnd));
}

double TrendAnalyse::getAvgSlops(const PeriodItemVec& prdItems)
{
	double avg = 0.0;

	auto iter = prdItems.begin();
	auto iterNext = prdItems.begin() + 1;
	while(iterNext != prdItems.end())
	{
		avg += PeriodItem::getSlopDiff(*iter, *iterNext);
		++iter;
		++iterNext;
	}

	avg /= (prdItems.size() - 1);

	return avg;
}

double TrendAnalyse::getAvgStandardDeviation(const PeriodItemVec& prdItems)
{
	double avg = 0.0;

	auto iter = prdItems.begin();
	auto iterNext = prdItems.begin() + 1;
	while(iterNext != prdItems.end())
	{
		avg += PeriodItem::getSDDiff(*iter, *iterNext);
		++iter;
		++iterNext;
	}

	avg /= (prdItems.size() - 1);

	return avg;
}