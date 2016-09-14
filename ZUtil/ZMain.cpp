#include "ZUtils.h"
#include "Sort.h"

class A{
public:
	int a;
};

class MyThread : public ZUtil::Thread{
public:
	 int Run();
};

int MyThread::Run()
{
	for (int i = 0; i < 10; i++){
		Sleep(1000);
		printf("[%d]\n", i);
	}
	return 0;
}

void Load(){
	//int x = GetPrivateProfileIntA("set", "x", -1, "d:\\x.ini");
	//x++;
	char sz[10];
	GetPrivateProfileStringA("set", "x4", "0.0", sz, 10, "d:\\x.ini");
	double d = atof(sz);
}

class 学生
{
public:
	int 获取年纪();
	int 年纪;
};

int 学生::获取年纪(){
	return 年纪;
}



int main(){	
	char* ptr = GETTIME();
	ZUtil::Log::Instance()->Write(ZUtil::Log::Log_Debug, __FUNCTION__, "HELLO:%s", "Jack");
	ZUtil::Log::Instance()->Write(ZUtil::Log::Log_Info, __FUNCTION__, "HELLO:%s", "Lucy");
	ZUtil::Log::Instance()->Write(ZUtil::Log::Log_Error, __FUNCTION__, "HELLO:%s", "FFF");
	printf("");

	//ZUtil::string strSrc(std::string("00012304560789"));
	//std::vector<std::string> vec;
	//strSrc.Split("0", vec);
	//while (1){
	//	Sleep(1000);
	//}


	//ZUtil::stringex str(10);
	//ZUtil::stringex str2("Hello");
	//std::vector<ZUtil::stringex> vec;
	//str2.Split("e", vec);
	//str2.Lower();
	//ZUtil::stringex str3("He", 2);
	//ZUtil::stringex str4(str3);
	//Load();
	//{
	//	ZUtil::FindFile findFile("\\\\192.168.199.239\\Program Files\\*.*");
	//	ZUtil::FileAttribute file("E:\\OS_ISO\\GhostXP_SP3_YLMF_DVD_ZJB_2016_0704.iso");
	//}
	system("pause");
}