#include "StdAfx.h"
#include "StringUtils.h"
#include <sstream>

using namespace std;


StringUtils::StringUtils(void)
{
}


StringUtils::~StringUtils(void)
{
}


void StringUtils::splitString(const std::string& src, const std::string& separator , std::vector<std::string>& dest)
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

std::string StringUtils::volumeToString(const std::string& symbol, double volume)
{
	if(symbol.find("SS_") != string::npos || symbol.find("SZ_") != string::npos)
	{
		volume = volume / 100;
		string result = formatWithCommas((ULONGLONG)volume);
		return result + " hands";
	}
	else
	{
		return formatWithCommas((ULONGLONG)volume);
	}
	
}
