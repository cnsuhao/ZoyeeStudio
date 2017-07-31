// curl_demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "HttpMod.h"

int main(int argc, char** argv){
	std::vector<std::string> vec;
	CHttpResponse rs = CHttpMod::Post("http://localhost:8080/test.php", "user=1234");
	CHttpResponse rs2 = CHttpMod::Get("http://localhost:8080/test.php?user=1234");
	CHttpMod::Get("http://localhost:8080/hello.txt");
	//CHttpMod::Download("https://download.tortoisegit.org/tgit/2.4.0.0/TortoiseGit-2.4.0.2-64bit.msi", vec, R"(d:\1234.msi)");
	return 0;
}
