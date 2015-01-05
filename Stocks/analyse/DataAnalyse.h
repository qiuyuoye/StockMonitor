#pragma once

#include "Analyse.h"
#include "stock/Stock.h"


class __declspec(dllexport) DataAnalyse : public Analyse
{
public:

	explicit DataAnalyse(StockCPtr stockPtr, IndexType indexType);
	~DataAnalyse(void);

protected:
	virtual void onAnalyse();

	virtual void printResult();

private:
	IndexType m_indexType;

	double m_maxVol;

	double m_minVol;

	double m_avgVol;
};

