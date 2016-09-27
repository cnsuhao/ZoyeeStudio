#include "ZUtils.h"


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
//http://blog.csdn.net/dongyewolong/article/details/8182055
//https://lug.ustc.edu.cn/sites/qtguide/

ZUtil::WaitModel wm(3);

int ThreadFunc(){
	printf("[%d]", wm.MulWait(3000));
	return 0;
}

int main(){	
	ZUtil::TextHttpServer server;
	server.Start(1000);
	ZUtil::WaitModel model;
	model.Wait(999999);

	//ZUtil::Md5 md5;
	//char sz[33];
	//md5.Code("hello", 5, sz);

	/*ZUtil::ZipModule zip;
	zip.OpenZip("x:\\1.zip", "x:\\222");
	int n = zip.UnZip();
	*/
	std::vector<std::string> vec;
	vec.push_back("YZ-xiaopiplayer.exe");




	ZUtil::DebugView dv(vec);
	dv.Start(false);
	std::string strProcessName;
	while(1){
		std::cin >> strProcessName;
		dv.AddProcessName((char*)strProcessName.c_str());
	}


	//ZUtil::stringex str("AABBCCAABBDD");
	//str.Replace("AA", "X");

	//ZUtil::stringex str2("33333");
	//str2+=str;
	//ZUtil::DebugView debugView;
	//debugView.AddPid(4768);
	//debugView.ListenDebugView();
	//ZUtil::ServiceManage service;
	//printf("[%d]", service.CreateKernelService("d:\\xpzsData\\ZQ\\xpstacks\\1\\ZQ-Hypervisor-x86-01.sys", "zqstHdDrv01", "zqplatform Hypervisor 01"));
	//int nPid = ZUtil::Process::GetPid("Code.exe");
	//ZUtil::Log::Instance()->Write(ZUtil::Log::Log_Debug, __FUNCTION__, "HELLO:%s", "Jack");
	//ZUtil::Log::Instance()->Write(ZUtil::Log::Log_Info, __FUNCTION__, "HELLO:%s", "Lucy");
	//ZUtil::Log::Instance()->Write(ZUtil::Log::Log_Error, __FUNCTION__, "HELLO:%s", "FFF");

	//ZUtil::stringex str;
	//ZUtil::stringex va = str.Format(1024, "%s", "Jack");

	//printf("");
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