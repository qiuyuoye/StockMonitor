#pragma once

#include <string>
#include <vector>


class USE_DLL StringUtils
{
public:
	static void splitString(const std::string& src, const std::string& separator , std::vector<std::string>& dest);


	static std::string volumeToString(const std::string& symbol, double volume);


	template<class T>
	static std::string formatWithCommas(T number);

private:
	StringUtils(void);
	~StringUtils(void);
};

template<class T>
std::string StringUtils::formatWithCommas(T number)
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << number;
	return ss.str();
}
