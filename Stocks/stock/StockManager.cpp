#include "StdAfx.h"
#include "StockManager.h"
#include "utils/FileUtils.h"
#include "StockRequest.h"
#include "StockConfig.h"
#include <sstream>
#include <time.h>
#include "utils/TimeUtils.h"

using namespace std;


StockManager::StockManager(void)
	: m_initialized(false)
{
}


StockManager::~StockManager(void)
{
	clear();
}

void StockManager::initialize()
{
	clear();
	vector<string> stockFiles;
	FileUtils::getInstance().listFiles("data", stockFiles, false, false);
	for(vector<string>::iterator iter = stockFiles.begin(); iter != stockFiles.end(); ++iter)
	{
		if(iter->find(".csv") == string::npos)
		{
			continue;
		}
		string symbol = iter->substr(0, iter->size() - 4);
		Stock *pStock = new Stock(symbol);
		m_stocksMap[symbol] = pStock;
	}
	m_initialized = true;
}

void StockManager::clear()
{
	for(StocksMap::iterator iter = m_stocksMap.begin(); iter != m_stocksMap.end(); ++iter)
	{
		delete iter->second;
	}
	m_stocksMap.clear();
	m_initialized = false;
}

Stock* StockManager::getStock(const std::string& symbol)
{
	if(!m_initialized)
	{
		initialize();
	}

	StocksMap::iterator iter = m_stocksMap.find(symbol);
	if(iter != m_stocksMap.end())
	{
		return iter->second;
	}

	Stock *pStock = new Stock(symbol);
	pStock->update();
	if(!pStock->isValid())
	{
		delete pStock;
		Log("No such stock symbol: %s", symbol.c_str());
		return NULL;
	}
	pStock->save();
	m_stocksMap[symbol] = pStock;
	return pStock;
}

void StockManager::updateAll()
{
	if(!m_initialized)
	{
		initialize();
	}

	for(StocksMap::iterator iter = m_stocksMap.begin(); iter != m_stocksMap.end(); ++iter)
	{
		iter->second->update();
		iter->second->save();
	}
}