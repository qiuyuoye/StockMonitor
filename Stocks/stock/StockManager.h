#pragma once

#include <string>
#include <map>
#include "Stock.h"


class __declspec(dllexport) StockManager
{
public:
	static StockManager& getInstance()
	{
		static StockManager instance;
		return instance;
	}

	void initialize();

	void clear();

	void updateAll();

	StockCPtr getStock(const std::string& symbol);

private:
	typedef std::map<std::string, StockPtr> StocksMap;

	StockManager(void);
	~StockManager(void);

	StocksMap m_stocksMap;

	bool m_initialized;


};

