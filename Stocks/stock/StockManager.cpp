#include "StdAfx.h"
#include "StockManager.h"
#include "utils/FileUtils.h"
#include "StockRequest.h"
#include "StockConfig.h"
#include <sstream>
#include <time.h>
#include "utils/TimeUtils.h"
#include <memory>

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
	for(auto iter = stockFiles.begin(); iter != stockFiles.end(); ++iter)
	{
		if(iter->find(".csv") == string::npos)
		{
			continue;
		}
		string symbol = iter->substr(0, iter->size() - 4);
		auto pStock = std::make_shared<Stock>(symbol);
		m_stocksMap[symbol] = pStock;
	}
	m_initialized = true;
}

void StockManager::clear()
{
	m_stocksMap.clear();
	m_initialized = false;
}

StockCPtr StockManager::getStock(const std::string& symbol)
{
	if(!m_initialized)
	{
		initialize();
	}

	auto iter = m_stocksMap.find(symbol);
	if(iter != m_stocksMap.end())
	{
		return iter->second;
	}

	auto pStock = std::make_shared<Stock>(symbol);
	pStock->update();
	if(!pStock->isValid())
	{
		Log("No such stock symbol: %s", symbol.c_str());
		return nullptr;
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

	for(auto iter = m_stocksMap.begin(); iter != m_stocksMap.end(); ++iter)
	{
		iter->second->update();
		iter->second->save();
	}
}