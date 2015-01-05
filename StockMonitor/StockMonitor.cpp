// StockMonitor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stock/StockRequest.h"
#include <string>
#include "stock/StockManager.h"
#include "analyse/DataAnalyse.h"
#include "analyse/TrendAnalyse.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//StockManager::getInstance().getStock("SS_600030");
	//StockManager::getInstance().getStock("SS_600109");
	//StockManager::getInstance().getStock("SS_600369");
	//StockManager::getInstance().getStock("SS_600873");
	//StockManager::getInstance().getStock("SS_600999");
	//StockManager::getInstance().getStock("SS_601099");
	//StockManager::getInstance().getStock("SS_601377");
	//StockManager::getInstance().getStock("SS_601555");
	//StockManager::getInstance().getStock("SS_601688");
	//StockManager::getInstance().getStock("SS_601788");
	//StockManager::getInstance().getStock("SS_601901");
	//StockManager::getInstance().updateAll();

	StockCPtr pStock = StockManager::getInstance().getStock("SS_600030");
	
	{
		DataAnalyse anaylse(pStock, E_VOLUME);
		anaylse.analyse();
	}
	{
		TrendAnalyse anaylse(pStock, E_ADJ_CLOSE);
		anaylse.pushRelative(E_VOLUME);
		anaylse.analyse();
	}

 	return 0;
}

