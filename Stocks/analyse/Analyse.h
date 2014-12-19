#pragma once

#include "stock/Stock.h"


class __declspec(dllexport) Analyse
{
public:
	Analyse(void);
	~Analyse(void);

	void analyse(const Stock* pStock);

protected:
	virtual void onAnalyse(const Stock* pStock){};

	virtual void printResult(const Stock* pStock){};
};

