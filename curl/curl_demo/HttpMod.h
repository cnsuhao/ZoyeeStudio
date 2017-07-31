#pragma once
#define CURL_STATICLIB
#include "include/curl/curl.h"
#include "include/curl/easy.h"
#include "include/curl/curlbuild.h"

#include <string>
#include <vector>

class CHttpResponse{
public:
	CHttpResponse(bool bSucc, std::string& strData) : m_bSucc(bSucc), m_strData(strData){};
	const bool m_bSucc;
	const std::string m_strData;
};

class MyCurl{
public:
	MyCurl(void* pCurl, void* pChunk);
	~MyCurl();
	void* pCurl;
	void* pChunk;
};

class CHttpMod{
public:
	static void InitEnvironment();
	static void UninitEnvironment();
	static CHttpResponse Post(const std::string& strUrl, const std::string& strFieldData, int nTimeout = 20);
	static CHttpResponse Get(const std::string& strUrl, const std::string& strUseAgent = std::string(""), int nTimeout = 20);
	static CHttpResponse Download(const std::string& strUrl, const std::vector<std::string>& vecHeader, const std::string& strFilePath, int nTimeout = 20);
	//static CHttpResponse FtpUpload();
	//static CHttpResponse FtpDownload();
private:
	static MyCurl* MakeCurl(const std::string& strUrl, std::vector<std::string>& vecHeader, std::string& strOutput);
};
