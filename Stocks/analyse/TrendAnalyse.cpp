#include "StdAfx.h"
#include "TrendAnalyse.h"
#include <algorithm>

using namespace std;

PVItem::PVItem()
	: m_pv(E_PV_NONE)
	, m_value(0.0)
	, m_index(0)
{

}

PVItem::~PVItem()
{

}

PeriodItem::PeriodItem()
	: m_period(E_PRD_NONE)
	, m_max(0.0)
	, m_min(0.0)
	, m_rate(0.0f)
	, m_startIndex(0)
	, m_endIndex(0)
{

}

PeriodItem::~PeriodItem()
{

}

TrendAnalyse::TrendAnalyse(void)
{
}


TrendAnalyse::~TrendAnalyse(void)
{
}

void TrendAnalyse::onAnalyse( const Stock* pStock )
{
	PVItemVec itemVec;
	vector<double> adjCloseVec;
	pStock->getAllAdjustClose(adjCloseVec);
	getPVs(adjCloseVec, itemVec);
	int j = 0;
}

void TrendAnalyse::printResult( const Stock* pStock )
{

}

void TrendAnalyse::getPVs(const std::vector<double>& datas, PVItemVec& items )
{
	items.clear();
	getDividePVs(datas, items);
	getFluctuatePVs(items);

}

static void initMaxMinTemp(PVItem& tempMax, PVItem& tempMin)
{
	tempMax.m_value = 0;
	tempMax.m_pv = E_PV_Peak;
	tempMax.m_index = 0;

	tempMin.m_value = 999999999999;
	tempMin.m_pv = E_PV_Valley;
	tempMin.m_index = 0;
}

static bool sortItemsByIndex(const PVItem& item1, const PVItem& item2)
{
	return item1.m_index < item2.m_index;
}

void TrendAnalyse::getDividePVs(const std::vector<double>& datas, PVItemVec& items )
{
	PVItem tempMax;
	PVItem tempMin;
	initMaxMinTemp(tempMax, tempMin);

	for(vector<double>::size_type i = 0; i < datas.size(); ++i)
	{

		if(datas[i] > tempMax.m_value)
		{
			tempMax.m_value = datas[i];
			tempMax.m_index = i;
		}

		if(datas[i] < tempMin.m_value)
		{
			tempMin.m_value = datas[i];
			tempMin.m_index = i;
		}

		if((i > 0 && (i + 1) % 5 == 0) || i == datas.size() - 1)
		{
			items.push_back(tempMax);
			items.push_back(tempMin);
			initMaxMinTemp(tempMax, tempMin);
		}

	}

	std::sort(items.begin(), items.end(), sortItemsByIndex);
}

bool invalidPVItem(const PVItem& item)
{
	return item.m_pv == E_PV_NONE;
}

void TrendAnalyse::getFluctuatePVs( PVItemVec& items )
{
	PVItemVec::iterator iter = items.begin();
	PVItemVec::iterator iterNext = iter + 1;
	while(iter != items.end() && iterNext != items.end())
	{
		if(iter->m_pv != iterNext->m_pv)
		{
			iter = iterNext;
			++iterNext;
			continue;
		}

		if(iter->m_pv == E_PV_Peak)
		{
			if(iter->m_value >= iterNext->m_value)
			{
				iterNext->m_pv = E_PV_NONE;
				++iterNext;
			}
			else
			{
				iter->m_pv = E_PV_NONE;
				iter = iterNext;
				++iterNext;
			}
			continue;
		}
		
		if(iter->m_pv == E_PV_Valley)
		{
			if(iter->m_value <= iterNext->m_value)
			{
				iterNext->m_pv = E_PV_NONE;
				++iterNext;
			}
			else
			{
				iter->m_pv = E_PV_NONE;
				iter = iterNext;
				++iterNext;
			}
			continue;
		}
	}


	PVItemVec::iterator it = std::remove_if (items.begin(), items.end(), invalidPVItem);
	items.resize(std::distance(items.begin(), it));
}

void TrendAnalyse::getPeriods( const PVItemVec& pvItems, PeriodItemVec& prdItems )
{

}

