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

int main(){	
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
	{
		ZUtil::FindFile findFile("X:\\1\\*.*");
		ZUtil::FileAttribute file("E:\\OS_ISO\\GhostXP_SP3_YLMF_DVD_ZJB_2016_0704.iso");
	}
	system("pause");
}