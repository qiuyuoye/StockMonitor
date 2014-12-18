#pragma once

#include <string>
#include <vector>


class __declspec(dllexport) FileUtils
{
public:
	static FileUtils& getInstance()
	{
		static FileUtils instance;
		return instance;
	}

	bool isFileEixst(const std::string& fileName);

	bool isAbsolutePath(const std::string& fileName);

	std::string getFullPath(const std::string& fileName);

	unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);

	void saveFile(const std::string& fileName, const std::string& content);

	void listFiles(const std::string& path, std::vector<std::string>& result, bool fullPath, bool subDir);

private:
	FileUtils(void);
	~FileUtils(void);

};

