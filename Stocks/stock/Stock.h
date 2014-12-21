#pragma once

#include <string>
#include <vector>
#include <wtypes.h>

typedef __declspec(dllexport) struct
{
	int year;
	int month;
	int day;
	double open;
	double high;
	double low;
	double close;
	ULONGLONG volume;
	double adjClose;
}StockItem;

typedef std::vector<StockItem> ItemVec;

#pragma warning(disable: 4251)
class __declspec(dllexport) Stock
{
public:
	explicit Stock(const std::string& symbol);
	~Stock(void);

	void update();

	void save();

	void getAllOpen(std::vector<double>& datas) const;

	void getAllHigh(std::vector<double>& datas) const;

	void getAllLow(std::vector<double>& datas) const;

	void getAllClose(std::vector<double>& datas) const;

	void getAllVolumn(std::vector<double>& datas) const;

	void getAllAdjustClose(std::vector<double>& datas) const;
	
	const std::string& getSymbol() const
	{
		return m_symbol;
	}

	bool isValid()
	{
		return m_vaild;
	}
	
	void setGroup(const std::string& group)
	{
		m_group = group;
	}

	const std::string& getGroup()
	{
		return m_group;
	}

	const ItemVec& getItems() const 
	{
		return m_items;
	}


private:
	void initWithFile();

	void initWithFileContent(const std::string& content);

	std::string getFileName();

	void insert(const std::string& content);

	bool isValidContent(const std::string& content);

	std::string toString();

	std::string m_symbol;

	std::string m_group;

	ItemVec m_items;

	bool m_vaild;
};

