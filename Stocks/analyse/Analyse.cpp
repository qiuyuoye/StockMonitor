#include "StdAfx.h"
#include "Analyse.h"


Analyse::Analyse(void)
{
}


Analyse::~Analyse(void)
{
}

void Analyse::analyse(const Stock* pStock)
{
	onAnalyse(pStock);
	printResult(pStock);
}