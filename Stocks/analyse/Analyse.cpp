#include "StdAfx.h"
#include "Analyse.h"


Analyse::Analyse(StockCPtr stockPtr)
	:m_stockPtr(stockPtr)
{
}


Analyse::~Analyse(void)
{
}

void Analyse::analyse()
{
	onAnalyse();
	printResult();
}