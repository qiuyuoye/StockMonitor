// StockMonitor.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "stock/StockRequest.h"
#include <string>
#include "stock/StockManager.h"


int _tmain(int argc, _TCHAR* argv[])
{
	StockManager::getInstance().getStock("SS_600030");
	StockManager::getInstance().getStock("SS_600109");
	StockManager::getInstance().getStock("SS_600369");
	StockManager::getInstance().getStock("SS_600873");
	StockManager::getInstance().getStock("SS_600999");
	StockManager::getInstance().getStock("SS_601099");
	StockManager::getInstance().getStock("SS_601377");
	StockManager::getInstance().getStock("SS_601555");
	StockManager::getInstance().getStock("SS_601688");
	StockManager::getInstance().getStock("SS_601788");
	StockManager::getInstance().getStock("SS_601901");
//	StockManager::getInstance().updateAll();
	return 0;
}
