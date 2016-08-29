#ifndef __ZUTIL_H__
#define __ZUTIL_H__

//  [8/12/2016 zoyee]
/*/////////////////////////////////////////////////////////////////////////
SystemInfo : [ 
	CpuInfo, DiskInfo, MemoryInfo, OSVerInfo
]

GetApplicationPath, ToChar, ToWChar, NewGuid...
Register : [
	RegEnum, ValueType
];

SocketConnect
	|__SocketServer
	|__SocketClient

LockKey
	|__Lock

ServiceManage

FileManage

/////////////////////////////////////////////////////////////////////////*/
#include <windows.h>
#include <iostream>
#define NO_USE_STL

#ifndef NO_USE_STL
#include <vector>
#include <string>
using namespace std;
#endif

namespace ZUtil{
	class SocketConnect;
};

typedef void(*pRecvCmd)(char* pRes, int nLen);
typedef void(*pCloseCallback)(char* pPath);
typedef void(*pNotify)(char* pData, int nLen, int nType, ZUtil::SocketConnect* pConnect);
typedef void(*pProtocol)(char* pData, int nLen, __out char* pOut, __out int& nOut);
typedef int(*pSend)(char* pData, int nLen, ZUtil::SocketConnect* pThis);
typedef int (*pThreadEvent)(char* pData, int nLen, int nEventType);

namespace ZUtil
{
	class SystemInfo
	{
	public:
		class CpuInfo{
		public:
			CpuInfo(bool bGetRightNow = false);
			int GetCore();
		private:
			void Get();
			int nCore;
			bool bGot;
		};
		
		class DiskInfo{
		public:
			enum DiskType{
				UnknownDiskType = 0,
				NoRootDirDiskType,
				UDiskType,
				PhyDiskType, 
				NetDiskType,
				CDRomDiskType,
				RamDiskType
			};
			DiskInfo(bool bGetRightNow = false);		
			~DiskInfo();
			class DiskItem{
			public:
				DiskItem();
				void Clear();
				DiskType type;
				char c;
				char fileSystem[32];
				char diskName[32];
				int Free;//bytes;
				int Max;//bytes;
			};

			bool GetDisk(int nIndex, DiskItem* item);
			int GetDiskCount();

		private:
			void Get();
			DiskItem* pItems;
			int nSize;
			bool bGot;
		};

		class MemoryInfo{
		public:
			MemoryInfo(bool bGetRightNow = false);
			int GetFree();
			int GetMax();

		private:
			void Get();
			int nFree;
			int nMax;
			bool bGot;
		};

		class OSVer{
		public:
			class OSVerInfo{
			public:
				char szDiscripe[32];
				bool bIsX64;
				int nWindowsDefineCode;
			};

			OSVer(bool bGetRightNow = false);
			OSVerInfo GetVer();

		private:
			void Get();
			bool bGot;
			OSVerInfo verInfo;
		};
	};

	class String{
	public:
		String(char* sz, int nLen);
		String(wchar_t* wsz, int nLen);
		String(int nValue);
		~String();

		int ToInt();
		char* ToString();
		int ToWChar(wchar_t* pRes);		
		int ToChar(char* pRes);
	protected:
		char* pData;
	};

	class Time{
	public:
		enum TimeFmt{
			yyyymmddhhmissms,
			yyyymmddhhmiss,
			yyyymmdd,
			hhmiss
		};
		Time();
		Time(TimeFmt fmt, char ymdConnector = '-', char hmsConnector = ':');
		char* GetTime();
		int GetSecondBalance(Time& obj);
		int operator-(Time& t2);
		TimeFmt GetFmt() ;//throw(char[]);

	protected:
		char szTime[128];
		TimeFmt fmt;
		char ymdConnector;
		char hmsConnector;
	};

	class Register{
	public:
		enum RegRootType{
			classRoot,
			currentUser,
			localMachine,
			users,
			currentConfig
		};

		enum emType{
			typeDword,
			typeStr,
			typeMultiStr
		};

		Register(char* path, bool bIsx64, RegRootType = localMachine);
		~Register();

		class RegEnum{
		public:
			RegEnum(char* szKey);
			~RegEnum();
			char* szKey;
			RegEnum* pNext;
		};
		RegEnum* GetKeys();
#ifndef NO_USE_STL
		int GetKeys(vector<string>& vecKeys);
#endif
		int SetKey(char* pKey);
		int DelKey(char* pKey);

		class ValueType{//仿variant
		public:
			ValueType();
			ValueType(char* pValue);
			ValueType(unsigned int nValue);
			ValueType(wchar_t* pValue);

			~ValueType();

			bool IsStr();
			bool IsDword();
			bool IsMultiStr();

			char* GetStr();
			unsigned int GetDword();
			wchar_t* GetMultiStr();
			emType GetType();

		private:
			void initValues();
			void reset();
			bool bStr;
			bool bDword;
			bool bMultiStr;
			char* pValue;
			unsigned int nValue;
			wchar_t* pwValue;
			emType nType;
		};

		int GetValue(char* pKey, emType type, ValueType& value);
		int SetValue(char* pKey, ValueType& value);
		int DelValue(char* pKey);

	private:
		int OpenKey();
		int DelKeyHelper(char* pKey, void* hSubKey, void* hParent);
		char* pPath;
		bool bIsx64;
		RegRootType regRootType;
		RegEnum* pKeys;
		void* hKey;
	};

	class Process{
	public:
		Process();
		static int Create(pCloseCallback pCallback, bool bWaitForClose, char* pPath, char* commandFmt, ...);
		static int CreateThenOutputCmd(__out char* pRes, int nResBuffLen, char* pPath, char* commandFmt, ...);
		static int CreateThenOutoutCmd(pRecvCmd pFunction, char* pPath, char* commandFmt, ...);//程序运行结束后会以回调的形式通知调用者

		static int CloseByWindowName(char* pszWindowsTitle, bool bWaitForClose = true);
		static int CloseByProcessName(char* pszProcessName, bool bWaitForClose = true);
		static int CloseByHwnd(HWND hWnd, bool bWaitForClose = true);	
#ifndef NO_USE_STL
		static int GetProcessList(vector<string>& vecProcessName);
#endif
		class ProcessInfo{
		public:
			ProcessInfo();
			~ProcessInfo();
			void Reset();
			char szProcessName[128];
			char szProcessCmd[1024];
			char szDescripe[1024];			
			int nUseMemory;//kb
			int nPid;
			int nThreads;
			ProcessInfo* pNext;
		};

		void LoadProcessInfo();
		ProcessInfo* GetProcessList();

	private:
		ProcessInfo* pInfo;

	protected:
		static int _Create(char* pCmd, HANDLE hWrite);
		static int _Create(char* pCmd, __out PROCESS_INFORMATION& pi);
	};

	class ServiceManage;

	class FileManage{
	public:
		FileManage();
		static void CreateDir(char* pDir);
		static bool RemoveDir(char* pDir);

		static int ReadFile(char* pFile, __out char* pRes, __in int nResLen);
		static int WriteFile(char* pFileName, char* pData, int nLen, bool bAppend = true);//bAppend追加
		static int DelFile(char* pFileName);
		static int MoveFile(char* pFileName, char* pNewFileName, bool bRemoveOld = false, bool bReWriteIsExist = false);//bRemoveOld 转移后是否要删除源文件, bReWriteIsExist 若目标地址有同名文件在, 是否尝试覆盖
	};

	class LockKey{
	public:
		LockKey();
		~LockKey();
		CRITICAL_SECTION* GetKey();
	protected:
		CRITICAL_SECTION cs;
	};
	
	class Lock{
	public:
		Lock(LockKey* pKey);
		~Lock();
	protected:
		LockKey* pKey;
	};

#define __KEY__ ZUtil::LockKey key;
#define __LOCK__ ZUtil::Lock lock(&key);

	//class AnnulusStore{//环形存储区
	//public:
	//	AnnulusStore(unsigned int nSize);
	//	~AnnulusStore();
	//	int GetData(char* pRes, int nLen);//足够返回0, 不够其他
	//	int WriteData(char* pData, int nLen);

	//protected:
	//	char* pData;
	//	unsigned int nBPos;
	//	unsigned int nEPos;
	//	unsigned int nMaxSize;
	//};

	class Log{
		Log(char* pLogPath, int nMaxLogFileSize = 10*1024);
		void WriteLog(int level, char* pModule, const char* pFmt, ...);		
	protected:
		__KEY__;
	};

	class Thread{
	public:
		Thread(void* pParam, pThreadEvent pFunc = nullptr);
		virtual int Run() = 0;
		void WaitForFinish();
		void Start();
		void Pause();
		void Resume();
		void Stop();
	protected:
		void* pParam;
	private:
		static DWORD WINAPI Work(void* pParam);
		static int DefaultCallback(char* pData, int nLen, int nEventType);		
		HANDLE hThread;
		pThreadEvent pFunc;
	};

	class SocketConnect{
	public:
		SocketConnect(int nType);
		virtual ~SocketConnect();
		unsigned int GetSocket();
		void SetSocket(unsigned int nSocket);
		char* GetIP();
		int GetPort();
		int GetType();

		virtual int InitConnectionParam(char* pIP, int nPort, pNotify pNotifyFunc);
		virtual int Connect() = 0;
		int DisConnect();

		void SetNotifyFunction(pNotify pNotifyFunc);//回调通知
		void SetDecodeProtocolFunc(pProtocol pDecodeProtocolFunc);//解码
		void SetEncodeProtocolFunc(pProtocol pEncodeProtocolFunc);//编码
		int Send(char* pBuff, int nLen);
				
		void SetPrea(SocketConnect* pPrea);
		void SetNext(SocketConnect* pNext);
		SocketConnect* GetNext();
		SocketConnect* GetPrea();

		pNotify GetNotify();
		pSend GetRecv();
	protected:
		static int CodeSend(char* pData, int nLen, SocketConnect* pThis);
		static int UnCodeSend(char* pData, int nLen, SocketConnect* pThis);
		static int RecvWithoutCallback(char* pData, int nLen, SocketConnect* pThis);
		static int RecvWithCallback(char* pData, int nLen, SocketConnect* pThis);
		static int RecvWithCallbackAndDecode(char* pData, int nLen, SocketConnect* pThis);
		static void DefaultCallback(char* pData, int nLen);
		void Reset();
		pSend pSendFunc;
		pSend pRecvFunc;
		pNotify pNotifyFunc;
		pProtocol pEncodeProtocolFunc;
		pProtocol pDecodeProtocolFunc;	
		unsigned int nSocket;
		char szIP[128];
		int nPort;
		int nType;
		SocketConnect* pPrea;
		SocketConnect* pNext;
	};

	class SocketServer : public SocketConnect{
	public:
		SocketServer(int nType);
		~SocketServer();
		virtual int InitConnectionParam(char* pIP, int nPort, pNotify pNotifyFunc);
		virtual int Connect();
		static DWORD WINAPI OnAcceptThread(void* pThis);
		static DWORD WINAPI OnRecvThread(void* pThis);
		void RemoveClient(SocketConnect* pClient);
	protected:
		void CheckEnCodeFunctions(SocketConnect* pConnect);
		SocketConnect* pClients;
		SocketConnect* pLastClient;
	};

	class SocketClient : public SocketConnect{
	public:
		SocketClient(int nType);
		virtual int Connect();
		SocketClient();
		~SocketClient();

		void SetServer(SocketConnect* pServer);
		SocketConnect* GetServer();
	protected:
		SocketConnect* pServer;
	};

	char* GetApplicationPath(bool bWithFileName);
	void ToChar(wchar_t* pwData, int nwLen, __out char* pData);
	void ToWchar(char* pData, int nLen, __out wchar_t* pwData);
	void NewGuid(__out char* pRes);
	char* NewGuid();
	const char* GetBuildDateTime();
	void SetPrivilegeDebug();
#ifndef NO_USE_STL
	std::string NewGuid();
#endif
}

#define ZSOCKET_ERROR -1
#define ZSOCKET_ACCEPT 0
#define ZSOCKET_CONNECTED 1
#define ZSOCKET_SEND 2
#define ZSOCKET_RECV 3
#define ZSOCKET_DISCONNECTED 4

#define SOCKET_UNKNOWN -1
#define SOCKET_CLIENT 0
#define SOCKET_SERVER 1
#define SOCKET_IOCP 2
#define SOCKET_SELECT 3
#define SOCKET_PIPE 4
#define SOCKET_SHAREMEMORY 5

#define FILE_NOT_EXIST -1
#define FILE_EXIST -2

#define THREAD_ERROR -1
#define THREAD_START 0
#define THREAD_PAUSE 1
#define THREAD_RESUME 2
#define THREAD_STOP 3
#define THREAD_FINISH 4
#define THREAD_RUNNING 5


#define SYSTEM_UNKNOWN              0        //未知版本
#define SYSTEM_XP                   1        //xp系统
#define SYSTEM_XP_SP3               2        //xp系统
#define SYSTEM_XP_64                3        //xp系统
#define SYSTEM_VISTA_32             4        //vista 32位系统
#define SYSTEM_VISTA_64             5        //vista 64位系统
#define SYSTEM_WIN7_32              6        //win7 32位系统
#define SYSTEM_WIN7_64              7        //win7 64位系统
#define SYSTEM_WIN8_32              8        //win8 32位系统
#define SYSTEM_WIN8_64              9        //win8 64位系统
#define SYSTEM_WIN8_1_32            10       //win8.1 32位系统
#define SYSTEM_WIN8_1_64            11       //win8.1 64位系统
#define SYSTEM_WIN10_32             12       //win10 32位系统
#define SYSTEM_WIN10_64             13       //win10 64位系统


#endif
