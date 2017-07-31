#include "stdafx.h"
#include "HttpMod.h"
#include <algorithm>
#include <xmemory>

#ifdef _DEBUG
#pragma comment(lib, "./lib_vc2013/libcurld.lib")
#else
#pragma comment(lib, "./lib_vc2013/libcurl.lib")
#endif

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")

namespace{
	class AutoInitCurl{
	public:
		AutoInitCurl(){
			CHttpMod::InitEnvironment();
		};
		~AutoInitCurl(){
			CHttpMod::UninitEnvironment();
		}
	};
	AutoInitCurl initCurl;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	std::string strData((char*)ptr, nmemb);
	std::string* pStrTotal = (std::string*)stream;
	*pStrTotal += strData;
	return nmemb * size;
}

size_t write_file(void *ptr, size_t size, size_t nmemb, void *stream){
	size_t written;
	FILE* pFile = (FILE*)stream;
	written = fwrite(ptr, size, nmemb, pFile);
	return written;
}

void CHttpMod::InitEnvironment()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

void CHttpMod::UninitEnvironment()
{
	curl_global_cleanup();
}

CHttpResponse CHttpMod::Post(const std::string& strUrl, const std::string& strFieldData, int nTimeout /*= 20*/)
{
	std::vector<std::string> vec;
	std::string strOutput;
	MyCurl* ptr = MakeCurl(strUrl, vec, strOutput);
	curl_easy_setopt(ptr->pCurl, CURLOPT_POST, 1);
	curl_easy_setopt(ptr->pCurl, CURLOPT_POSTFIELDS, strFieldData.c_str());
	curl_easy_setopt(ptr->pCurl, CURLOPT_TIMEOUT, nTimeout);
	CURLcode res = curl_easy_perform(ptr->pCurl);
	delete ptr;
	if (res != CURLE_OK){
		return CHttpResponse(false, std::string(curl_easy_strerror(res)));
	}else{
		return CHttpResponse(true, strOutput);
	}
}

CHttpResponse CHttpMod::Get(const std::string& strUrl, const std::string& strUseAgent /*= std::string("")*/, int nTimeout /*= 20*/)
{
	std::vector<std::string> vec;
	std::string strOutput;
	vec.push_back(strUseAgent);
	MyCurl* ptr = MakeCurl(strUrl, vec, strOutput);
	curl_easy_setopt(ptr->pCurl, CURLOPT_TIMEOUT, nTimeout);
	CURLcode res = curl_easy_perform(ptr->pCurl);
	delete ptr;
	if (res != CURLE_OK){
		return CHttpResponse(false, std::string(curl_easy_strerror(res)));
	}
	else{
		return CHttpResponse(true, strOutput);
	}
}

CHttpResponse CHttpMod::Download(const std::string& strUrl, const std::vector<std::string>& vecHeader, const std::string& strFilePath, int nTimeout /*= 20*/)
{
	CURL* pCurl = curl_easy_init();
	curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_file);
	FILE *fp = NULL;
	fopen_s(&fp, strFilePath.c_str(), "wb");
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);

	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
	curl_easy_setopt(pCurl, CURLOPT_ACCEPT_ENCODING, "deflate");

	struct curl_slist *pChunk = NULL;
	std::for_each(vecHeader.begin(), vecHeader.end(), [&](const std::string& header)
	{
		pChunk = curl_slist_append(pChunk, header.c_str());
	});

	CURLcode res = curl_easy_perform(pCurl);
	curl_easy_cleanup(pCurl);
	curl_slist_free_all(pChunk);
	fclose(fp);
	if (res != CURLE_OK){
		return CHttpResponse(false, std::string(curl_easy_strerror(res)));
	}else{
		return CHttpResponse(true, std::string(""));
	}
}

MyCurl* CHttpMod::MakeCurl(const std::string& strUrl, std::vector<std::string>& vecHeader, std::string& strOutput)
{
	CURL* pCurl = curl_easy_init();
	struct curl_slist *pChunk = nullptr;
	curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &strOutput);

	std::for_each(vecHeader.begin(), vecHeader.end(), [&](const std::string& strHeader){
		pChunk = curl_slist_append(pChunk, strHeader.c_str());
	});
	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pChunk);
	return new MyCurl(pCurl, pChunk);
}

MyCurl::MyCurl(void* pCurl, void* pChunk)
{
	this->pCurl = pCurl;
	this->pChunk = pChunk;
}

MyCurl::~MyCurl()
{
	if (pCurl){
		curl_easy_cleanup(pCurl);
	}
	if (pChunk){
		curl_slist_free_all((curl_slist*)pChunk);
	}
}
