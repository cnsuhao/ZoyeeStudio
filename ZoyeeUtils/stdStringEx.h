#ifndef __STD_STRING_EX_H__
#define __STD_STRING_EX_H__
//Create by Ã¨²è
#include <string>
#include <list>

namespace ZoyeeUtils{
	class string : public std::string{
	public:
		string(int nNumber);
		string();
		string(std::string& str);
		string(char* pText);
		string(string& str);
		string(const char* pFmt, ...);
		string(std::wstring& wstr);
		std::list<std::string> Split(std::string strSep = ",");
		std::string Replace(std::string src, std::string desc);
		static std::string Replace(std::string strText, std::string src, std::string desc);
		int ToInt();
		float toFloat();
		std::string ToStdString();
		std::wstring ToStdWString();
	};
}
#endif