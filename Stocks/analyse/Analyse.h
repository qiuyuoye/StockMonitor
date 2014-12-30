#pragma once

#include "stock/Stock.h"


class __declspec(dllexport) Analyse
{
public:
	explicit Analyse(StockCPtr stockPtr);
	~Analyse(void);

	void analyse();

protected:
	virtual void onAnalyse(){};

	virtual void printResult(){};

	StockCPtr m_stockPtr;
	
};

