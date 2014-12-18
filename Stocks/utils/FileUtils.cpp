#include "StdAfx.h"
#include "FileUtils.h"
#include <fstream>
#include <io.h>

using namespace std;

static std::string utf8Togbk(const char *src)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP,0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (strTemp.find('?') != std::string::npos)
	{
		strTemp.assign(src);
	}
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

static char s_rootDir[MAX_PATH] = {0};


FileUtils::FileUtils(void)
{
	WCHAR  wszPath[MAX_PATH] = {0};

	int nNum = WideCharToMultiByte(CP_ACP, 0, wszPath,
		GetCurrentDirectoryW(sizeof(wszPath), wszPath),
		s_rootDir, MAX_PATH, NULL, NULL);
	s_rootDir[nNum] = '\\';
}


FileUtils::~FileUtils(void)
{
}

bool FileUtils::isFileEixst(const std::string& fileName)
{
	std::string strFilePathAscii = utf8Togbk(fileName.c_str());
	if (0 == strFilePathAscii.length())
	{
		return false;
	}

	std::string strPath = strFilePathAscii;
	if (!isAbsolutePath(strPath))
	{ // Not absolute path, add the default root path at the beginning.
		strPath.insert(0, s_rootDir);
	}
	return GetFileAttributesA(strPath.c_str()) != -1 ? true : false;
}

bool FileUtils::isAbsolutePath(const std::string& fileName)
{
	std::string strPathAscii = utf8Togbk(fileName.c_str());
	if (strPathAscii.length() > 2 
		&& ( (strPathAscii[0] >= 'a' && strPathAscii[0] <= 'z') || (strPathAscii[0] >= 'A' && strPathAscii[0] <= 'Z') )
		&& strPathAscii[1] == ':')
	{
		return true;
	}
	return false;
}

unsigned char* FileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
	unsigned char * pBuffer = NULL;
	*pSize = 0;
	do
	{
		// read the file from hardware
		std::string fullPath = getFullPath(pszFileName);
		FILE *fp = NULL;
		fopen_s(&fp, fullPath.c_str(), pszMode);
		if(fp == NULL)
		{
			break;
		}

		fseek(fp,0,SEEK_END);
		*pSize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		pBuffer = new unsigned char[*pSize];
		*pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
		fclose(fp);
	} while (0);

	if (! pBuffer)
	{
		std::string msg = "Get data from file(";
		msg.append(pszFileName).append(") failed!");

		Log("%s", msg.c_str());
	}
	return pBuffer;
}

std::string FileUtils::getFullPath(const std::string& fileName)
{
	if (isAbsolutePath(fileName))
	{
		return fileName;
	}
	string path = s_rootDir + fileName;
	return path;
}

void FileUtils::saveFile(const std::string& fileName, const std::string& content)
{
	string fname = getFullPath(fileName);
	ofstream outfs;
	outfs.open(fname);
	outfs << content;
	outfs.close();
}

void FileUtils::listFiles(const std::string& path, std::vector<std::string>& result, bool fullPath, bool subDir)
{
	string realPath = getFullPath(path) + "\\*.*";
	struct _finddata_t dirFile;
	long hFile;

	if (( hFile = _findfirst(realPath.c_str(), &dirFile )) != -1L )
	{
		do
		{
			if ( !strcmp( dirFile.name, "."   )) continue;
			if ( !strcmp( dirFile.name, ".."  )) continue;
			if ( dirFile.attrib & _A_HIDDEN ) continue;
			if ( dirFile.attrib & _A_SUBDIR )
			{
				if(subDir)
				{
					listFiles(path + "/" + dirFile.name, result, fullPath, subDir);
					continue;	
				}
				else
				{
					continue;
				}		
			}

			if(fullPath)
			{
				result.push_back(realPath + "\\" + dirFile.name);
			}
			else
			{
				result.push_back(dirFile.name);
			}

		} while ( _findnext( hFile, &dirFile ) != -1L );
		_findclose( hFile );
	}

}