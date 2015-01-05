#pragma once

#include "stock/Stock.h"


class __declspec(dllexport) Analyse
{
public:
	explicit Analyse(StockCPtr stockPtr);
	~Analyse(void);

	virtual void analyse(){};

	virtual void print(){};

protected:

	StockCPtr m_pStock;
	
};

