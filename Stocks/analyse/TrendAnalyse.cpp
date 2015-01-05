#include "StdAfx.h"
#include "TrendAnalyse.h"
#include <algorithm>
#include "utils/MathUtil.h"
#include "utils/StringUtils.h"

using namespace std;


double PeriodItem::getSlopDiff(const PeriodItem& item1, const PeriodItem& item2)
{
	if((item1.m_avgSlope > 0.0 && item2.m_avgSlope < 0.0)
		|| (item1.m_avgSlope < 0.0 && item2.m_avgSlope > 0.0))
	{
		return abs(item1.m_avgSlope - item2.m_avgSlope) * 1.1;
	}
	return abs(item1.m_avgSlope - item2.m_avgSlope);
}

double PeriodItem::getSDDiff(const PeriodItem& item1, const PeriodItem& item2)
{
	return MathUtil::difference(item1.m_slopSD, item2.m_slopSD);
}

double PeriodItem::getFluctuationDiff(const PeriodItem& item1, const PeriodItem& item2)
{
	return MathUtil::difference(item1.m_fluctuation, item2.m_fluctuation);
}

PeriodItem::PeriodItem(float ratio)
	: m_period(E_TRD_NONE)
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
	: m_period(E_TRD_NONE)
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
	m_avg = 0.0;
	m_avgSlope = 0.0;
	m_slopSD = 0.0;
	m_maxGain = 0.0;
	m_maxLoss = 0.0;
	m_fluctuation = 0.0;
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
		m_period = m_avgSlope > 0.01 ? E_TRD_UP : E_TRD_FLUCTUATION_UP;
	}
	else
	{
		m_period = m_avgSlope < -0.01 ? E_TRD_DOWN : E_TRD_FLUCTUATION_DOWN;
	}

	double increase = 0.0;
	for(unsigned int i = m_startIndex; i < m_endIndex; ++i)
	{
		double x = (datas[i + 1] - datas[i]);

		if(x > 0.0f)
		{
			m_maxGain += x;
			++increase;
		}
		else
		{
			m_maxLoss += x;
		}
	}

	m_fluctuation = increase / (m_endIndex - m_startIndex);
}

void PeriodItem::print(StockCPtr pStock, int offset)
{
	const ItemVec& items = pStock->getItems();

	Log("%d-%d-%d  %d-%d-%d: Tread %s, Max %s, Min %s, Avg %s, Max Gain %s, Max Loss %s, Slop %s, Standard Deviation %s", 
		items[m_startIndex + offset].year, items[m_startIndex + offset].month, items[m_startIndex + offset].day,
		items[m_endIndex + offset].year, items[m_endIndex + offset].month, items[m_endIndex + offset].day,
		getPeriod().c_str(),
		StringUtils::formatWithCommas(m_max).c_str(), StringUtils::formatWithCommas(m_min).c_str(), StringUtils::formatWithCommas(m_avg).c_str(),
		StringUtils::formatWithCommas(m_maxGain).c_str(), StringUtils::formatWithCommas(m_maxLoss).c_str(),
		StringUtils::formatWithCommas(m_avgSlope).c_str(), StringUtils::formatWithCommas(m_slopSD).c_str());
	
}

std::string PeriodItem::getPeriod()
{
	switch(m_period)
	{
	case E_TRD_UP:
		return "UP";
	case E_TRD_FLUCTUATION_UP:
		return "FLUCTUATION_UP";
	case E_TRD_DOWN:
		return "DOWN";
	case E_TRD_FLUCTUATION_DOWN:
		return "FLUCTUATION_DOWN";
	}
	return "";
}

TrendAnalyse::TrendAnalyse(StockCPtr stockPtr, IndexType majorIndex)
	: Analyse(stockPtr)
	, m_majorIdx(majorIndex)
	, m_relIdxVec()
	, m_mergedSize(-1)
{
}


TrendAnalyse::~TrendAnalyse(void)
{
}

void TrendAnalyse::analyse()
{
	analysisMajor();

	analyseRelPeriods();

}

void TrendAnalyse::print()
{
	for(UINT i = 0; i < m_majorVec.size(); ++i)
	{
		Log("----------------Stock Item----------------");
		m_majorVec[i].print(m_pStock, 0);
		for(auto iter = m_relPrdsMap.begin(); iter != m_relPrdsMap.end(); ++iter)
		{
			Log("Stock Item: %s", Stock::indexTypeToString(iter->first).c_str());
			iter->second[i].print(m_pStock, 0);
		}
	}

}

void TrendAnalyse::analysisMajor()
{
	vector<double> majorVec;
	m_pStock->getDatasByIndex(majorVec, m_majorIdx);

	if(majorVec.size() <= 1)
	{
		Log("The size of datas must bigger than 1");
		return;
	}

	int size = m_mergedSize;
	if(size < 0)
	{
		size = majorVec.size() / 15;
	}
	initMajorPrds(majorVec);
	mergePeriods(majorVec, size);

}

void TrendAnalyse::initMajorPrds(const std::vector<double>& datas)
{
	double minData = 0;
	double maxData = 0;
	for(UINT i = 0; i < datas.size(); ++i)
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
		item.m_period = (datas[index] > datas[i] ? E_TRD_UP : E_TRD_DOWN);
		item.m_startIndex = i;
		item.m_endIndex = index;

		while(index < datas.size() && index + 1 < datas.size())
		{
			if((item.m_period == E_TRD_UP && datas[index] < datas[index + 1])
				|| (item.m_period == E_TRD_DOWN && datas[index] >= datas[index + 1]))
			{

				item.m_endIndex = index + 1;
				++index;
			}
			else
			{
				break;
			}
		}

		m_majorVec.push_back(item);

		i = item.m_endIndex;
		
	}

	for_each(m_majorVec.begin(), m_majorVec.end(), [&datas](PeriodItem& item){
		item.compute(datas);
	});
}

void TrendAnalyse::mergePeriods(const std::vector<double>& datas, unsigned int size)
{
	while(m_majorVec.size() >= size)
	{
		mergeSimilarPeriods(datas);
	}
}

void TrendAnalyse::mergeSimilarPeriods(const std::vector<double>& datas)
{
	if(m_majorVec.empty())
	{
		return;
	}

	vector<DiffItem> slopDiffs;
	for(auto iter = m_majorVec.begin(); iter != m_majorVec.end(); ++iter)
	{
		auto iterNext = iter + 1;
		if(iterNext != m_majorVec.end())
		{
			DiffItem item;
			item.m_index = distance(m_majorVec.begin(), iter);
			item.m_value = PeriodItem::getSlopDiff(*iter, *iterNext);
			slopDiffs.push_back(item);
		}
	}

	DiffItem minItem = slopDiffs[0];
	for_each(slopDiffs.begin(), slopDiffs.end(), [&minItem](const DiffItem& item)
	{
		if(item.m_value < minItem.m_value)
		{
			minItem = item;
		}
	});

	m_majorVec.at(minItem.m_index).m_endIndex = m_majorVec.at(minItem.m_index + 1).m_endIndex;
	m_majorVec.at(minItem.m_index).compute(datas);
	m_majorVec.at(minItem.m_index + 1).m_period = E_TRD_NONE;

	auto iterEnd = std::remove_if(m_majorVec.begin(), m_majorVec.end(), [](const PeriodItem& item){
		return item.m_period == E_TRD_NONE;
	});

	m_majorVec.resize(std::distance(m_majorVec.begin(), iterEnd));
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

void TrendAnalyse::analyseRelPeriods()
{
	m_relPrdsMap.clear();

	for(auto iter = m_relIdxVec.begin(); iter != m_relIdxVec.end(); ++iter)
	{
		vector<double> dataVec;
		m_pStock->getDatasByIndex(dataVec, *iter);

		m_relPrdsMap[*iter] = m_majorVec;
		auto iterRelPrd = m_relPrdsMap.find(*iter);
		for_each(iterRelPrd->second.begin(), iterRelPrd->second.end(), [&dataVec](PeriodItem& item){
			item.compute(dataVec);
		});
	}

}