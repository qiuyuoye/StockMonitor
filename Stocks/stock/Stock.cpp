#include "StdAfx.h"
#include "Stock.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include "utils/TimeUtils.h"
#include "utils/FileUtils.h"
#include "StockRequest.h"
#include "StockConfig.h"

using namespace std;

static bool itemSort(const StockItem& item1, const StockItem& item2)
{
	if(item1.year != item2.year)
	{
		return item1.year < item2.year;
	}
	if(item1.month != item2.month)
	{
		return item1.month < item2.month;
	}

	return item1.day < item2.day;;
}

static bool itemCompare(const StockItem& item1, const StockItem& item2)
{
	return item1.year == item2.year && item1.month == item2.month && item1.day == item2.day;
}


void splitString(const std::string& src, const std::string& separator , std::vector<std::string>& dest)
{
	string::size_type start = 0, index;
	string substring;
	do
	{
		index = src.find(separator,start);
		if (index != string::npos)
		{    
			if(index == start)
			{
				index = start + separator.length();
				start += separator.length();
				continue;
			}
			substring = src.substr(start, index - start);
			dest.push_back(substring);
			start = index + separator.size();
			index = src.find(separator,start);
		}
		else
		{
			if(start == 0)
			{
				substring = src.substr(start);
				dest.push_back(substring);
			}
			return;
		}
	}while(index != string::npos);

	//the last token
	substring = src.substr(start);
	dest.push_back(substring);
}

Stock::Stock(const std::string& symbol)
	: m_symbol(symbol)
	, m_vaild(true)
{
	initWithFile();
}


Stock::~Stock(void)
{
}

void Stock::initWithFile()
{
	string filename = getFileName();
	if(!FileUtils::getInstance().isFileEixst(filename))
	{
		return;
	}

	unsigned long size = 0;
	unsigned char* pData = 0;
	unsigned char* pBuffer = FileUtils::getInstance().getFileData(filename.c_str(), "rb", &size);
	ostringstream oss;
	oss << pBuffer;

	initWithFileContent(oss.str());
}

void Stock::initWithFileContent(const std::string& content)
{
	istringstream iss(content);
	string line;
	vector<string> lineVec;
	vector<string> dateVec;
	int lineHeader = 0;

	while (getline(iss, line)) 
	{
		switch(lineHeader)
		{
		case 0:
			{
				++lineHeader;
				splitString(line, ",", lineVec);
				if(lineVec.size() == 1)
				{
					m_group = lineVec[0];
					++lineHeader;
				}
				lineVec.clear();
				dateVec.clear();
			}
			break;
		case 1:
			{
				++lineHeader;
			}
			break;
		case 2:
			{
				do 
				{
					splitString(line, ",", lineVec);
					if(lineVec.size() != 7)
					{
						Log("Invalid stock line %s", line.c_str());
						break;
					}

					StockItem item;
					splitString(lineVec[0], "-", dateVec);
					if(dateVec.size() != 3)
					{
						Log("Invalid date %s", lineVec[0].c_str());
						break;
					}

					item.year = atoi(dateVec[0].c_str());
					item.month = atoi(dateVec[1].c_str());
					item.day = atoi(dateVec[2].c_str());
					item.open = atof(lineVec[1].c_str());
					item.high = atof(lineVec[2].c_str());
					item.low = atof(lineVec[3].c_str());
					item.close = atof(lineVec[4].c_str());
					sscanf_s(lineVec[5].c_str(), "%llu", &(item.volume));
					item.adjClose = atof(lineVec[6].c_str());
					m_items.push_back(item);

				} while (false);

				lineVec.clear();
				dateVec.clear();
			}
		}
	}

	std::sort(m_items.begin(), m_items.end(), itemSort);
}

std::string Stock::getFileName()
{
	string filename = "data/" + m_symbol + ".csv";
	return filename;
}

void Stock::update()
{
	bool needUpdate = true;
	time_t timeNow = TimeUtils::getSystemMilliSecond() / 1000;
	
	string content;
	struct tm* pTimeInfo = localtime(&timeNow);
	if(m_items.empty())
	{
		content = StockRequest::requestHistory(m_symbol, 
			StockConfig::START_YEAR, StockConfig::START_MONTH, StockConfig::START_DAY,
			pTimeInfo->tm_year + 1900, pTimeInfo->tm_mon + 1, pTimeInfo->tm_mday);
	}
	else
	{
		ItemVec::reverse_iterator iter = m_items.rbegin();
		content = StockRequest::requestHistory(m_symbol,
			iter->year, iter->month, iter->day,
			pTimeInfo->tm_year + 1900, pTimeInfo->tm_mon + 1, pTimeInfo->tm_mday);
	}

	if(!isValidContent(content))
	{
		Log("Stock: not a valid content!");
		m_vaild = false;
		return;
	}

	insert(content);
}

bool Stock::isValidContent(const std::string& content)
{
	if(content.empty())
	{
		return false;
	}

	if(content[0] == '{' && content[content.size() - 1] == '}')
	{
		return false;
	}

	return true;
}

void Stock::insert(const std::string& content)
{
	istringstream iss(content);
	string line;
	vector<string> lineVec;
	vector<string> dateVec;
	bool lineHeader = true;

	while (getline(iss, line)) 
	{
		if(lineHeader)
		{
			lineHeader = false;
			continue;
		}

		do 
		{
			splitString(line, ",", lineVec);
			if(lineVec.size() != 7)
			{
				if(line.size() > 500)
				{
					line = line.substr(0, 500);
				}

				Log("Invalid stock line %s", line.c_str());
				break;
			}

			StockItem item;
			splitString(lineVec[0], "-", dateVec);
			if(dateVec.size() != 3)
			{
				if(line.size() > 500)
				{
					line = line.substr(0, 500);
				}
				Log("Invalid date %s", line.c_str());
				break;
			}

			item.year = atoi(dateVec[0].c_str());
			item.month = atoi(dateVec[1].c_str());
			item.day = atoi(dateVec[2].c_str());
			item.open = atof(lineVec[1].c_str());
			item.high = atof(lineVec[2].c_str());
			item.low = atof(lineVec[3].c_str());
			item.close = atof(lineVec[4].c_str());
			sscanf_s(lineVec[5].c_str(), "%llu", &(item.volume));
			item.adjClose = atof(lineVec[6].c_str());
			m_items.push_back(item);

		} while (false);

		lineVec.clear();
		dateVec.clear();

	}

	std::sort(m_items.begin(), m_items.end(), itemSort);
	ItemVec::iterator it = std::unique (m_items.begin(), m_items.end(), itemCompare);
	m_items.resize(std::distance(m_items.begin(), it));

}

void Stock::save()
{
	if(!m_vaild)
	{
		return;
	}

	FileUtils::getInstance().saveFile(getFileName(), toString());
}

std::string Stock::toString()
{
	ostringstream oss;
	oss << m_group;
	oss << "\n";
	oss << "Date,Open,High,Low,Close,Volume,Adjusted Close\n";
	for(ItemVec::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
	{
		oss << iter->year;
		oss << "-";
		oss << iter->month;
		oss << "-";
		oss << iter->day;
		oss << ",";
		oss << iter->open;
		oss << ",";
		oss << iter->high;
		oss << ",";
		oss << iter->low;
		oss << ",";
		oss << iter->close;
		oss << ",";
		oss << iter->volume;
		oss << ",";
		oss << iter->adjClose;
		oss << "\n";
	}
	return oss.str();
}