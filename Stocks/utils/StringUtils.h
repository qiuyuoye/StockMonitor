#pragma once

#include <string>
#include <vector>


class USE_DLL StringUtils
{
public:
	static void splitString(const std::string& src, const std::string& separator , std::vector<std::string>& dest);

private:
	StringUtils(void);
	~StringUtils(void);
};

