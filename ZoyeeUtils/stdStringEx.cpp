#include "stdStringEx.h"
#include <stdarg.h>
#include <Windows.h>
using namespace ZoyeeUtils;


ZoyeeUtils::string::string( int nNumber ) : std::string()
{
	resize(16);
	sprintf((char*)data(), "%d", nNumber);
}

std::list<std::string> ZoyeeUtils::string::Split( std::string strSep /*= ","*/ )
{
	std::list<std::string> lst;
	string strTemp = data();
	int nSepLen = strlen(strSep.c_str());
	int nPos = strTemp.find(strSep);
	while(nPos != -1){
		lst.push_back(strTemp.substr(0, nPos));
		strTemp.erase(0, nPos + nSepLen);
		nPos = strTemp.find(strSep);
	}
	if ( ! strTemp.empty()){
		lst.push_back(strTemp);
	}
	return lst;
}

std::string ZoyeeUtils::string::Replace( std::string src, std::string desc )
{
	int nPos = find(src);
	while(nPos != -1){
		string strHead = substr(0, nPos);
		strHead += desc;
		erase(0, nPos + strlen(src.c_str()));
		insert(0, strHead);
		nPos = find(src);
	}
	return *this;
}

std::string ZoyeeUtils::string::Replace( std::string strText, std::string src, std::string desc )
{
	int nPos = strText.find(src);
	while(nPos != -1){
		string strHead = strText.substr(0, nPos);
		strHead += desc;
		strText.erase(0, nPos + strlen(src.c_str()));
		strText.insert(0, strHead);
		nPos = strText.find(src);
	}
	return strText;
}

int ZoyeeUtils::string::ToInt()
{
	return atoi(data());
}

float ZoyeeUtils::string::toFloat()
{
	return atof(data());
}

std::string ZoyeeUtils::string::ToStdString()
{
	return *this;
}

std::wstring ZoyeeUtils::string::ToStdWString()
{
	std::wstring wstr;
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, (char*)data(), -1, NULL, NULL);
	if (nLen <= 0){
		return L"";
	}
	wstr.resize(nLen);
	MultiByteToWideChar(CP_ACP, 0, (char*)data(), -1, (wchar_t*)wstr.data(), nLen);
	return wstr;
}

string::string() : std::string(){

}

string::string(std::string& str) : std::string(str){

}

string::string(string& str) : std::string(){
	resize(str.size());
	memcpy((void*)data(), str.data(), str.size());
}

string::string(char* pText) : std::string(pText){

}

string::string(const char* pFmt, ...) : std::string(){
	va_list ap;
	va_start(ap, pFmt);
	int nPrintLen = vprintf(pFmt, ap);	
	resize(nPrintLen);
	vsprintf((char*)data(), pFmt, ap);
	va_end(ap);
}

string::string(std::wstring& wstr) : std::string(){
	std::string str;
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstr.c_str(), -1, 0, 0, 0, 0);
	if (nLen > 0){
		resize(nLen);
		WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wstr.c_str(), -1, (char*)data(), nLen, 0, 0);
	}
}