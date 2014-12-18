#include "StdAfx.h"
#include "StockRequest.h"

#include <string>

#include <Windows.h>
#include <Iphlpapi.h>
#include <wininet.h>
#include <shellApi.h>
#pragma comment(lib,"wininet")
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

const std::string QUANDL_TOKEN = "VyrhmHKJ18XpKy2mcviN";
const std::string QUANDL_HISTORY_URL="http://www.quandl.com/api/v1/datasets/YAHOO/%s.csv?&trim_start=%d-%d-%d&trim_end=%d-%d-%d&sort_order=asc&auth_token=VyrhmHKJ18XpKy2mcviN";
const std::string QUANDL_QUERY_URL="https://www.quandl.com/api/v1/datasets.json?query=%s&auth_token=VyrhmHKJ18XpKy2mcviN";

static bool getHttpResponse(const string& url, string& response)
{
	HINTERNET hInternet1 = 
		InternetOpenA(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if (NULL == hInternet1)
	{
		InternetCloseHandle(hInternet1);
		return false;
	}

	HINTERNET hInternet2 = 
		InternetOpenUrlA(hInternet1, url.c_str(),NULL,NULL,INTERNET_FLAG_NO_CACHE_WRITE,NULL);
	if (NULL == hInternet2)
	{
		InternetCloseHandle(hInternet2);
		InternetCloseHandle(hInternet1);
		return false;
	}

	char *pool = new char[1024 * 1024 * 4]();
	unsigned long buffer;
	if(!InternetReadFile(hInternet2, pool, 1024 * 1024 * 4, &buffer))
	{
		return false;
	}


	response = pool;

	delete pool;

	return true;
}

StockRequest::StockRequest(void)
{
}


StockRequest::~StockRequest(void)
{
}

std::string StockRequest::requestHistory(const std::string& stockSymbol, int startYear, int startMon, int startDay, int endYear, int endMon, int endDay)
{
	string resp;
	char formarUrl[160];
	sprintf_s(formarUrl, QUANDL_HISTORY_URL.c_str(), stockSymbol.c_str(), startYear, startMon, startDay, endYear, endMon, endDay);
	Log("Request %s", formarUrl);
	if(!getHttpResponse(formarUrl, resp))
	{
		resp="";
	}
	return resp;
}