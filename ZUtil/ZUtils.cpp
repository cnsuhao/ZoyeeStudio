#include "ZUtils.h"

//#include <WinSock2.h>
#include <Windows.h>
#include <ObjBase.h>
#include <io.h>
#include <tlhelp32.h>
#include <string>
#include <psapi.h>    
#include <direct.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Ole32.lib")//com
#pragma comment(lib, "Advapi32.lib")//reg
#pragma comment(lib,"psapi.lib")  //psapi
#pragma comment(lib, "user32.lib")//findwindow

#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

#define ClientIn "client is in!"
#define ClientInLen strlen(ClientIn)
#define ClientDisconnect "client is disconnect!"
#define ClientDisconnectLen strlen(ClientDisconnect)
#define ClientError "client is error!"
#define ClientErrorLen strlen(ClientError)

//-------cpu.begin--------

ZUtil::SystemInfo::CpuInfo::CpuInfo(bool bGetRightNow) : bGot(false)
{
	if (bGetRightNow)
	{
		Get();
	}
}

void ZUtil::SystemInfo::CpuInfo::Get()
{
	::SYSTEM_INFO si;
	GetSystemInfo(&si);
	this->nCore = si.dwNumberOfProcessors;
	bGot = true;
}

int ZUtil::SystemInfo::CpuInfo::GetCore()
{
	if ( ! bGot)
	{
		Get();
	}
	return nCore;
}

ZUtil::SystemInfo::DiskInfo::DiskInfo(bool bGetRightNow) : pItems(nullptr), bGot(false)
{
	if (bGetRightNow)
	{
		Get();
	}
}

void ZUtil::SystemInfo::DiskInfo::Get()
{
	DiskItem item;
	int nLength = GetLogicalDriveStrings(0, NULL);
	char* DiskStr = new char[nLength];
	GetLogicalDriveStringsA(nLength, DiskStr);
	DWORD dwSerialNumber, dwMaxComponentLength, dwFsFlags;
	_ULARGE_INTEGER lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
	int nDiskCount = nLength > 0 ? nLength / 4: 0;
	if (nDiskCount <= 0)
	{
		return;
	}
	nSize = nDiskCount;
	pItems = new DiskItem[nDiskCount];

	for (int i = 0; i < nDiskCount; i++)
	{
		item.Clear();
		char szDirveName[] = {DiskStr[i * 4], ':', '\\', 0};
		item.type = (DiskType)GetDriveTypeA(szDirveName);
		item.c = DiskStr[i * 4];

		if( ! GetVolumeInformationA(szDirveName, 
			item.diskName, sizeof(item.diskName),
			&dwSerialNumber, &dwMaxComponentLength, &dwFsFlags, 
			item.fileSystem, sizeof(item.fileSystem)))
			continue;

		if( ! GetDiskFreeSpaceExA(szDirveName, &lpFreeBytesAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes))
			continue;

		item.Free = lpTotalNumberOfFreeBytes.QuadPart / (1024 * 1024);
		item.Max = lpTotalNumberOfBytes.QuadPart / (1024 * 1024);

		pItems[i] = item;
	}

	delete[] DiskStr;
	bGot = true;
}

bool ZUtil::SystemInfo::DiskInfo::GetDisk( int nIndex, DiskItem* item )
{
	if (nIndex > nSize)
	{
		return false;
	}
	if ( ! bGot)
	{
		Get();
	}
	*item = pItems[nIndex];
	return true;
}

int ZUtil::SystemInfo::DiskInfo::GetDiskCount()
{
	if ( ! bGot)
	{
		Get();
	}
	return nSize;
}

ZUtil::SystemInfo::DiskInfo::~DiskInfo()
{
	if (pItems)
	{
		delete[] pItems;
		pItems = nullptr;
	}
}

void ZUtil::SystemInfo::DiskInfo::DiskItem::Clear()
{
	type = UnknownDiskType;
	c  = 0;
	memset(fileSystem, 0, 32);
	memset(diskName, 0, 32);
	Free = Max = 0;
}

ZUtil::SystemInfo::DiskInfo::DiskItem::DiskItem()
{
	Clear();
}

ZUtil::SystemInfo::MemoryInfo::MemoryInfo( bool bGetRightNow /*= true*/ ) : bGot(false)
{
	if (bGetRightNow)
	{
		Get();
	}
}

int ZUtil::SystemInfo::MemoryInfo::GetFree()
{
	if ( ! bGot)
	{
		Get();
	}
	return nFree;
}

int ZUtil::SystemInfo::MemoryInfo::GetMax()
{
	if ( ! bGot)
	{
		Get();
	}
	return nMax;
}

void ZUtil::SystemInfo::MemoryInfo::Get()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	nFree = statex.ullAvailPhys / 1024 / 1024;
	nMax = statex.ullTotalPhys / 1024 / 1024;
	bGot = true;
}

typedef void (WINAPI *pFnGetNativeSystemInfo)(SYSTEM_INFO* lpSystemInfo);
pFnGetNativeSystemInfo pFun = (pFnGetNativeSystemInfo)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetNativeSystemInfo");  

ZUtil::SystemInfo::OSVer::OSVer( bool bGetRightNow /*= false*/ ) : bGot(false)
{
	if (bGetRightNow)
	{
		Get();
	}
}

ZUtil::SystemInfo::OSVer::OSVerInfo ZUtil::SystemInfo::OSVer::GetVer()
{
	if ( ! bGot)
	{
		Get();
	}
	return verInfo;
}

void ZUtil::SystemInfo::OSVer::Get()
{
	bGot = true;
	SYSTEM_INFO si = { 0 };  

	if (pFun != NULL)
	{
		pFun(&si);
	}
	else
	{
		GetSystemInfo(&si);
	}

	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		verInfo.bIsX64 = true;
	}
	else
		verInfo.bIsX64 = false;

	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	BOOL bRet = GetVersionEx((OSVERSIONINFO*)&osvi);
	if(FALSE == bRet)
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		bRet = GetVersionEx((OSVERSIONINFO*)&osvi);
	}

	if(FALSE == bRet)
	{
		verInfo.nWindowsDefineCode = SYSTEM_UNKNOWN;
		strcpy(verInfo.szDiscripe, "Unknown");
	}

	verInfo.nWindowsDefineCode = SYSTEM_UNKNOWN;

	if(VER_PLATFORM_WIN32_NT == osvi.dwPlatformId)
	{
		if(6 == osvi.dwMajorVersion && 0 == osvi.dwMinorVersion)
		{
			if(VER_NT_WORKSTATION == osvi.wProductType)
			{
				verInfo.nWindowsDefineCode = verInfo.bIsX64 == true ? SYSTEM_VISTA_64 : SYSTEM_VISTA_32;     
				strcpy(verInfo.szDiscripe, verInfo.bIsX64 == true ? "Vista-x64" : "Vista-x86");
			}
		}
		else if(5 == osvi.dwMajorVersion && 1 == osvi.dwMinorVersion)
		{
			verInfo.nWindowsDefineCode = SYSTEM_XP;
			strcpy(verInfo.szDiscripe, "XP-x86");
			if(3 == osvi.wServicePackMajor && 0 == osvi.wServicePackMinor)
			{
				verInfo.nWindowsDefineCode = SYSTEM_XP_SP3;
				strcpy(verInfo.szDiscripe, "XP-SP3-x86");
			}
		}
		else if(5 == osvi.dwMajorVersion && 2 == osvi.dwMinorVersion)
		{
			if((VER_NT_WORKSTATION == osvi.wProductType) && (true == verInfo.bIsX64))
			{
				verInfo.nWindowsDefineCode = SYSTEM_XP_64;
				strcpy(verInfo.szDiscripe, "XP-x64");
			}
		}
		else if(6 == osvi.dwMajorVersion && 1 == osvi.dwMinorVersion)
		{
			if(VER_NT_WORKSTATION == osvi.wProductType)
			{
				verInfo.nWindowsDefineCode = verInfo.bIsX64 == true ? SYSTEM_WIN7_64 : SYSTEM_WIN7_32;
				strcpy(verInfo.szDiscripe, verInfo.bIsX64 == true ? "Win7-x64" : "Win7-x86");
			}
		}
		else if(6 == osvi.dwMajorVersion && 2 == osvi.dwMinorVersion)
		{
			if(VER_NT_WORKSTATION == osvi.wProductType)
			{
				verInfo.nWindowsDefineCode = verInfo.bIsX64 == true ? SYSTEM_WIN8_64 : SYSTEM_WIN8_32;
				strcpy(verInfo.szDiscripe, verInfo.bIsX64 == true ? "Win8-x64" : "Win8-x86");
			}
		}
		else if(6 == osvi.dwMajorVersion && 3 == osvi.dwMinorVersion)
		{
			if(VER_NT_WORKSTATION == osvi.wProductType)
			{
				verInfo.nWindowsDefineCode = verInfo.bIsX64 == true ? SYSTEM_WIN8_1_64 : SYSTEM_WIN8_1_32;
				strcpy(verInfo.szDiscripe, verInfo.bIsX64 == true ? "Win8_1-x64" : "Win8_1-x86");
			}
		}
		else if(10 == osvi.dwMajorVersion && 0 == osvi.dwMinorVersion)
		{
			if(VER_NT_WORKSTATION == osvi.wProductType)
			{
				verInfo.nWindowsDefineCode = verInfo.bIsX64 == true ? SYSTEM_WIN10_64 : SYSTEM_WIN10_32;
				strcpy(verInfo.szDiscripe, verInfo.bIsX64 == true ? "Win10-x64" : "Win10-x86");
			}
		}
	}
}

char* ZUtil::GetApplicationPath( bool bWithFileName )
{
	static char _szPath[1024] = "";
	GetModuleFileNameA(NULL, _szPath, 1024);
	if ( ! bWithFileName)
	{
		char* p = strrchr(_szPath, '\\');
		*p = 0;
	}
	return _szPath;
}

void ZUtil::ToChar( wchar_t* pwData, int nwLen, __out char* pData )
{
	WideCharToMultiByte(CP_ACP, 0, pwData, -1, pData, (nwLen * 2), nullptr, nullptr);
}

void ZUtil::ToWchar( char* pData, int nLen, __out wchar_t* pwData )
{
	MultiByteToWideChar(CP_ACP, 0, pData, -1, pwData, (nLen * 2));
}
//#ifndef NO_USE_STL
//std::string ZUtil::NewGuid()
//{
//	char szGuid[64] = {0};
//	GUID guid;
//	if (S_OK == ::CoCreateGuid(&guid))
//	{
//		sprintf(szGuid, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X", 
//			guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], 
//			guid.Data4[5], guid.Data4[6], guid.Data4[7]);
//		return (szGuid);
//	}
//	return "";
//}
//#endif

void ZUtil::NewGuid( __out char* pRes )
{
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		sprintf(pRes, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X", 
			guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], 
			guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}	
}

char* ZUtil::NewGuid(){
	static char szGuid[128] = "";
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		sprintf(szGuid, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X", 
			guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], 
			guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}	
	return szGuid;
}

void ZUtil::SetPrivilegeDebug()
{
	HANDLE hHandle;
	TOKEN_PRIVILEGES tkp;
	ZeroMemory(&tkp,sizeof(TOKEN_PRIVILEGES));

	do 
	{
		if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hHandle))
		{
			break;;
		}
		if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid))
		{   
			break;;
		}
		tkp.PrivilegeCount=1;
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		if(!AdjustTokenPrivileges(hHandle,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0))
		{
			break;;
		}
	} while (false);	
}

const char* ZUtil::GetBuildDateTime()
{
	return __DATE__" "__TIME__;
}

ZUtil::Register::Register( char* path, bool bIsx64, RegRootType regRootType )
{
	pPath = nullptr;
	this->regRootType = regRootType;
	this->bIsx64 = bIsx64;
	this->hKey = nullptr;
	pKeys = nullptr;
	if (path != nullptr)
	{
		int nLen = strlen(path);
		pPath = new char[nLen + 1];
		strcpy(&pPath[0], path);
	}
}

ZUtil::Register::~Register()
{
	if (pPath)
	{
		delete[] pPath;
		pPath = nullptr;
	}
	if (pKeys)
	{
		delete pKeys;
		pKeys = nullptr;
	}
	if (hKey)
	{
		RegCloseKey((HKEY)hKey);
	}
}

ZUtil::Register::RegEnum* ZUtil::Register::GetKeys()
{
	if (OpenKey() == ERROR_SUCCESS)
	{
		const int nSize = 128;
		char szKey[nSize];
		int nIndex = 0;

		if (pKeys)
		{
			delete pKeys;
			pKeys = nullptr;
		}
		unsigned int nOutLen;
		if (RegEnumKeyExA((HKEY)hKey, nIndex, szKey, (LPDWORD)&nOutLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			pKeys = new RegEnum(szKey);
			nIndex++;
			nOutLen = nSize;
		}
		RegEnum* pCurrent = pKeys;
		while(RegEnumKeyExA((HKEY)hKey, nIndex, szKey, (LPDWORD)&nOutLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS){
			RegEnum* pTemp = new RegEnum(szKey);
			pCurrent->pNext = pTemp;
			pCurrent = pTemp;
			nIndex++;
			nOutLen = nSize;
		}
	}
	return pKeys;
}
#ifndef NO_USE_STL
int ZUtil::Register::GetKeys( vector<string>& vecKeys )
{
	if (hKey == nullptr)
	{
		if (OpenKey() != ERROR_SUCCESS)
		{
			return 0;
		}
	}

	vecKeys.clear();
	int nIndex = 0;
	char szKey[128];
	while(RegEnumKeyA((HKEY)hKey, nIndex, szKey, 128) == ERROR_SUCCESS){
		nIndex++;
		vecKeys.push_back(szKey);
	}
	return vecKeys.size();
}
#endif
int ZUtil::Register::OpenKey()
{
	if (hKey)
	{
		RegCloseKey((HKEY)hKey);
		hKey = nullptr;
	}
	HKEY hRootKey;
	switch(this->regRootType)
	{
	case classRoot:
		hRootKey = HKEY_CLASSES_ROOT;
		break;
	case currentUser:
		hRootKey = HKEY_CURRENT_USER;
		break;
	case localMachine:
		hRootKey = HKEY_LOCAL_MACHINE;
		break;
	case users:
		hRootKey = HKEY_USERS;
		break;
	case currentConfig:
		hRootKey = HKEY_CURRENT_CONFIG;
		break;
	default:
		hRootKey = 0;
	}

	return RegOpenKeyExA((HKEY)hRootKey, pPath, 0, (KEY_ALL_ACCESS | (bIsx64 ? KEY_WOW64_64KEY : 0)), (PHKEY)&hKey);
}

int ZUtil::Register::SetKey( char* pKey )
{
	if (hKey == nullptr)
	{
		if (OpenKey() != ERROR_SUCCESS)
		{
			return GetLastError();
		}
	}
	HKEY hSubKey;
	return (RegCreateKeyA((HKEY)hKey, (LPCSTR)pKey, &hSubKey) == ERROR_SUCCESS ? 0 : GetLastError());
}

int ZUtil::Register::GetValue( char* pKey, emType type, ZUtil::Register::ValueType& value)
{
	if (hKey == nullptr)
	{
		if(OpenKey() != ERROR_SUCCESS)
			return GetLastError();
	}

	DWORD dwSize, dwType;
	switch(type)
	{
	case typeDword:
		{
			DWORD dw;
			dwSize = sizeof(dw);
			dwType = REG_DWORD;
			if (RegQueryValueExA((HKEY)hKey, pKey, 0, &dwType, (LPBYTE)&dw, &dwSize) == ERROR_SUCCESS)
			{
				ValueType _value(dw);
				value = _value;
				return 0;
			}			
			break;
		}
	case typeStr:
		{
			char szValue[1024] = "";
			dwType = REG_SZ;
			if (RegQueryValueExA((HKEY)hKey, pKey, 0, &dwType, (LPBYTE)&szValue, &dwSize) == ERROR_SUCCESS)
			{
				ValueType _value(szValue);
				value = _value;
				return 0;
			}
			break;
		}
	case typeMultiStr:
		{
			break;
		}
	default:
		break;
	}
	return GetLastError();
}

int ZUtil::Register::SetValue( char* pKey, ValueType& value )
{
	if (hKey == nullptr)
	{
		if(OpenKey() != ERROR_SUCCESS)
			return GetLastError();
	}

	DWORD* pdwValue = nullptr;
	DWORD dwType, dwSize;

	switch(value.GetType())
	{
	case typeDword:
		{
			DWORD dw = value.GetDword();
			pdwValue = &dw;
			dwType = REG_DWORD;
			dwSize = sizeof(DWORD);
			break;
		}
	case typeStr:
		{
			pdwValue = (DWORD*)value.GetStr();
			dwType = REG_SZ;
			dwSize = strlen(value.GetStr());
			break;
		}		
	case typeMultiStr:
		break;
	default:
		break;
	}

	if (RegSetValueExA((HKEY)hKey, pKey, 0, dwType, (const BYTE *)pdwValue, dwSize) == ERROR_SUCCESS)
	{
		return 0;
	}

	return GetLastError();
}

int ZUtil::Register::DelValue( char* pKey )
{
	if (hKey == nullptr)
	{
		if(OpenKey() != ERROR_SUCCESS)
			return GetLastError();
	}

	if(RegDeleteValueA((HKEY)hKey, pKey) == ERROR_SUCCESS)
	{
		return 0;
	}
	return GetLastError();
}

int ZUtil::Register::DelKeyHelper( char* pKey, void* hCurrentKey, void* hParent )
{
	char szKey[128];	
	int nIndex = 0;
	HKEY hSubKey;
	while(RegEnumKeyA((HKEY)hCurrentKey, nIndex, szKey, 128) == ERROR_SUCCESS)
	{
		RegOpenKeyExA((HKEY)hCurrentKey, szKey, 0, (KEY_ALL_ACCESS | (bIsx64 ? KEY_WOW64_64KEY : 0)), &hSubKey);
		DelKeyHelper(szKey, hSubKey, hCurrentKey);		
	}
	return RegDeleteKeyA((HKEY)hParent, pKey);
}

int ZUtil::Register::DelKey( char* pKey )
{
	if (hKey == nullptr)
	{
		if (OpenKey() != ERROR_SUCCESS)
		{
			return GetLastError();
		}
	}

	HKEY hSubKey;
	char szKey[128];
	int nIndex = 0;
	while(RegEnumKeyA((HKEY)hKey, nIndex, szKey, 128) == ERROR_SUCCESS)
	{
		nIndex++;
		if (strcmp(szKey, pKey) == 0)
		{
			if(RegOpenKeyExA((HKEY)hKey, pKey, 0, (KEY_ALL_ACCESS | (bIsx64 ? KEY_WOW64_64KEY : 0)), &hSubKey) == ERROR_SUCCESS)
				return DelKeyHelper(pKey, hSubKey, hKey);
		}
	}
	return 0;
}

ZUtil::Register::RegEnum::~RegEnum()
{
	if (szKey)
	{
		delete[] szKey;
	}
	if (pNext)
	{
		delete pNext;
	}
}

ZUtil::Register::RegEnum::RegEnum( char* szKey )
{
	this->pNext = nullptr;
	this->szKey = nullptr;
	if (szKey)
	{
		int nLen = strlen(szKey);
		this->szKey = new char[nLen + 1];
		strcpy(&this->szKey[0], szKey);
	}
}

void ZUtil::Register::ValueType::initValues()
{
	bDword = bStr = bMultiStr = false;
	pValue = 0;
	nValue = 0;
	pwValue = 0;
}

bool ZUtil::Register::ValueType::IsStr()
{
	return bStr;
}

bool ZUtil::Register::ValueType::IsDword()
{
	return bDword;
}

bool ZUtil::Register::ValueType::IsMultiStr()
{
	return bMultiStr;
}

char* ZUtil::Register::ValueType::GetStr()
{
	return pValue;
}

unsigned int ZUtil::Register::ValueType::GetDword()
{
	return nValue;
}

wchar_t* ZUtil::Register::ValueType::GetMultiStr()
{
	return pwValue;
}

ZUtil::Register::ValueType::~ValueType()
{
	initValues();
}

ZUtil::Register::ValueType::ValueType( char* pValue )
{
	initValues();
	if (pValue)
	{
		int nLen = strlen(pValue);
		this->pValue = new char[nLen + 1];
		strcpy(&this->pValue[0], pValue);
		bStr = true;
		nType = typeStr;
	}
}

ZUtil::Register::ValueType::ValueType(unsigned int nValue)
{
	initValues();
	this->nValue = nValue;
	bDword = true;
	nType = typeDword;
}

ZUtil::Register::ValueType::ValueType(wchar_t* pValue)
{
	initValues();
	if (pValue)
	{
		int nLen = wcslen(pValue);
		pwValue = new wchar_t[nLen + 1];
		memset(pwValue, 0, (nLen + 1)* 2);
		memcpy(pwValue, pValue, (nLen) * 2);
	}
	bMultiStr = true;
	nType = typeMultiStr;
}

ZUtil::Register::ValueType::ValueType()
{
	initValues();
}

ZUtil::Register::emType ZUtil::Register::ValueType::GetType()
{
	return nType;
}

void ZUtil::Register::ValueType::reset()
{
	if (pValue)
	{
		delete[] pValue;
		pValue = nullptr;
	}
	if (pwValue)
	{
		delete[] pwValue;
		pwValue = nullptr;
	}

	bStr = bMultiStr = bDword = false;
}



struct SPipeThreadData
{
	pRecvCmd pFunc;
	HANDLE hRead;
};

int PipeThread(void* ptr)
{
	SPipeThreadData* pData = (SPipeThreadData*)ptr;
	
	const int nMaxLen = 128;
	char sz[nMaxLen + 1];
	DWORD nReadBytes;
	while(ReadFile(pData->hRead, sz, nMaxLen, &nReadBytes, NULL) != NULL){
		sz[nReadBytes] = 0;
		pData->pFunc(sz, nReadBytes);
	}
	CloseHandle(pData->hRead);
	delete pData;
	return 0;
}

int ZUtil::Process::CreateThenOutoutCmd( pRecvCmd pFunction, char* pPath, char* commandFmt, ... )
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;  
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.lpSecurityDescriptor = NULL;  //使用系统默认的安全描述符 
	sa.bInheritHandle = TRUE;  //创建的进程继承句柄	
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return GetLastError();
	}

	char szFull[1024] = "";
	char szCmd[1024] = "";
	va_list ap;
	va_start(ap, commandFmt);
	vsprintf(szCmd, commandFmt, ap);
	sprintf(szFull, "\"%s\" %s", pPath, szCmd);

	if (_Create(szFull, hWrite) != 0)
	{
		return GetLastError();
	}

	SPipeThreadData* pSTData = new SPipeThreadData;
	pSTData->hRead = hRead;
	pSTData->pFunc = pFunction;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)PipeThread, pSTData, 0, 0);
	CloseHandle(hWrite);
	return 0;
}

int ZUtil::Process::Create( pCloseCallback pCallback, bool bWaitForClose, char* pPath, char* commandFmt, ... )
{
	char szFull[1024] = "";
	char szCmd[1024] = "";
	va_list ap;
	va_start(ap, commandFmt);
	vsprintf(szCmd, commandFmt, ap);
	sprintf(szFull, "\"%s\" %s", pPath, szCmd);

	PROCESS_INFORMATION pi;
	int nRes = _Create(szFull, pi); 
	if (nRes != 0)
	{
		return nRes;
	}

	if (bWaitForClose)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		if (pCallback != nullptr)
		{
			pCallback(szFull);
		}
	}
	return 0;
}

int ZUtil::Process::_Create( char* pCmd, HANDLE hWrite )
{
	STARTUPINFOA si; 
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si); 
	GetStartupInfoA(&si); 	
	if (hWrite != nullptr)
	{
		si.hStdOutput = hWrite;  //新创建进程的标准输出连在写管道一端
	}
	si.wShowWindow = SW_HIDE;  //隐藏窗口 
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!CreateProcessA(NULL, pCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)){//创建子进程
		return GetLastError();
	}  
	return 0;
}

int ZUtil::Process::_Create( char* pCmd, __out PROCESS_INFORMATION& pi )
{
	STARTUPINFOA si; 
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si); 
	GetStartupInfoA(&si); 	
	si.wShowWindow = SW_HIDE;  //隐藏窗口 
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	if (!CreateProcessA(NULL, pCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)){//创建子进程
		return GetLastError();
	}  
	return 0;
}
#ifndef NO_USE_STL
int ZUtil::Process::GetProcessList( vector<string>& vecProcessName )
{

	return 0;
}

#endif
ZUtil::Process::ProcessInfo* ZUtil::Process::GetProcessList()
{
	LoadProcessInfo();	
	return pInfo;
}

void ZUtil::Process::LoadProcessInfo()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	if (pInfo != nullptr)
	{
		delete pInfo;
	}
	ProcessInfo* pCurrent = nullptr, *pPrea = nullptr;
	PROCESS_MEMORY_COUNTERS pmc;
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	if (bMore)
	{
		pInfo = new ProcessInfo;
		ToChar(pe32.szExeFile, wcslen(pe32.szExeFile), pInfo->szProcessName);
		pInfo->nPid = pe32.th32ProcessID;
		pInfo->nThreads = pe32.cntThreads;
		pPrea = pInfo;
	}

	while(::Process32Next(hProcessSnap, &pe32))
	{
		HANDLE hProcess = OpenProcess(	PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);		
		if ( ! hProcess)
		{
			CloseHandle(hProcess);
			continue;
		}

		pCurrent = new ProcessInfo;
		ToChar(pe32.szExeFile, wcslen(pe32.szExeFile), pCurrent->szProcessName);
		GetProcessMemoryInfo(hProcess,&pmc,sizeof(pmc));
		pCurrent->nUseMemory = pmc.WorkingSetSize / 1024;
		CloseHandle(hProcess);

		pCurrent->nPid = pe32.th32ProcessID;
		pCurrent->nThreads = pe32.cntThreads;
		pPrea->pNext = pCurrent;
		pPrea = pCurrent;		
	}
	CloseHandle(hProcessSnap);
}

ZUtil::Process::Process()
{
	pInfo = nullptr;
}

int ZUtil::Process::CreateThenOutputCmd( __out char* pRes, int nResBuffLen, char* pPath, char* commandFmt, ... )
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;  
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.lpSecurityDescriptor = NULL;  //使用系统默认的安全描述符 
	sa.bInheritHandle = TRUE;  //创建的进程继承句柄	
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return GetLastError();
	}

	char szFull[1024] = "";
	char szCmd[1024] = "";
	va_list ap;
	va_start(ap, commandFmt);
	vsprintf(szCmd, commandFmt, ap);
	sprintf(szFull, "\"%s\" %s", pPath, szCmd);

	if (_Create(szFull, hWrite) != 0)
	{
		return GetLastError();
	}

	DWORD nReadBytes;
	while(ReadFile(hRead, pRes, nResBuffLen, &nReadBytes, NULL) != NULL){
		pRes[nReadBytes] = 0;
		break;
	}
	CloseHandle(hRead);
	return 0;
}

int ZUtil::Process::CloseByWindowName( char* pszWindowsTitle, bool bWaitForClose /*= true*/ )
{
	SetPrivilegeDebug();
	HWND hFindWindow;
	int nCount = 0;
	hFindWindow = FindWindowA(nullptr, pszWindowsTitle);
	while(hFindWindow){		
		CloseByHwnd(hFindWindow, bWaitForClose);
		hFindWindow = GetNextWindow(hFindWindow, GW_HWNDNEXT);
		nCount++;
	}
	return nCount;
}

int ZUtil::Process::CloseByProcessName( char* pszProcessName, bool bWaitForClose /*= true*/ )
{
	SetPrivilegeDebug();
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	int nCount = 0;
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	char szProcessName[128];
	while(bMore){
		ToChar(pe32.szExeFile, wcslen(pe32.szExeFile), szProcessName);
		if (stricmp(szProcessName, pszProcessName) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
			if (hProcess)
			{
				TerminateProcess(hProcess, 0);
				nCount++;
				if (bWaitForClose)
				{
					WaitForSingleObject(hProcess, INFINITE);
				}
				CloseHandle(hProcess);
			}
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	return nCount;
}

int ZUtil::Process::CloseByHwnd( HWND hWnd, bool bWaitForClose /*= true*/ )
{
	SetPrivilegeDebug();
	DWORD dwThreadId = 0;
	GetWindowThreadProcessId(hWnd, &dwThreadId);
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwThreadId);
	TerminateProcess(hProcess, 0);
	if (bWaitForClose)
	{
		WaitForSingleObject(hProcess, INFINITE);
	}		
	return 0;
}

ZUtil::Process::ProcessInfo::ProcessInfo()
{
	Reset();
	pNext = nullptr;
}

void ZUtil::Process::ProcessInfo::Reset()
{
	memset(szProcessName, 0, 128);
	memset(szProcessCmd, 0, 1024);
	memset(szDescripe, 0, 1024);
	nUseMemory = 0;
	nPid = 0;
	nThreads = 0;
}

ZUtil::Process::ProcessInfo::~ProcessInfo()
{
	if (pNext)
	{
		delete pNext;
		pNext = nullptr;
	}
}

ZUtil::SocketConnect::SocketConnect( int nType )
{
	Reset();
	this->nType = nType;
}

void ZUtil::SocketConnect::Reset()
{
	nType = SOCKET_UNKNOWN;
	nSocket = 0;
	memset(szIP, 0, 128);
	nPort = 0;
	pPrea = pNext = nullptr;

	pSendFunc = UnCodeSend;
	pRecvFunc = RecvWithoutCallback;
	pNotifyFunc = nullptr;
	pEncodeProtocolFunc = nullptr;
	pDecodeProtocolFunc = nullptr;	
}

ZUtil::SocketConnect::~SocketConnect()
{
	if (pNext)
	{
		pNext->pPrea = pPrea;
	}
	else
	{
		if (pPrea)
		{
			pPrea->SetNext(nullptr);
		}
	}
	if (pPrea)
	{
		pPrea->pNext = pNext;
	}
	else
	{
		if (pNext)
		{
			pNext->SetPrea(nullptr);
		}
	}
	DisConnect();
}

unsigned int ZUtil::SocketConnect::GetSocket()
{
	return nSocket;
}

char* ZUtil::SocketConnect::GetIP()
{
	return szIP;
}

int ZUtil::SocketConnect::GetPort()
{
	return nPort;
}

int ZUtil::SocketConnect::InitConnectionParam( char* pIP, int nPort, pNotify pNotifyFunc )
{
	strcpy(szIP, pIP);
	this->nPort = nPort;
	this->SetNotifyFunction(pNotifyFunc);
	return 0;
}

int ZUtil::SocketConnect::DisConnect()
{
	return closesocket(nSocket);
}

void ZUtil::SocketConnect::SetNotifyFunction( pNotify pNotifyFunc )
{
	this->pNotifyFunc = pNotifyFunc;
	if (pNotifyFunc)
	{
		pRecvFunc = RecvWithCallback;
	}
	else
	{
		pRecvFunc = pDecodeProtocolFunc == nullptr ? RecvWithoutCallback : RecvWithCallbackAndDecode; 
	}
}

void ZUtil::SocketConnect::SetDecodeProtocolFunc( pProtocol pDecodeProtocolFunc )
{
	this->pDecodeProtocolFunc = pDecodeProtocolFunc;
	pRecvFunc = RecvWithCallbackAndDecode;
}

int ZUtil::SocketConnect::CodeSend( char* pData, int nLen, SocketConnect* pThis )
{
	char szOut[1024];
	int nOutLen = 0;
	pThis->pEncodeProtocolFunc(pData, nLen, szOut, nOutLen);
	return send(pThis->GetSocket(), szOut, nOutLen, 0);
}

int ZUtil::SocketConnect::UnCodeSend( char* pData, int nLen, SocketConnect* pThis )
{
	return send(pThis->GetSocket(), pData, nLen, 0);
}

int ZUtil::SocketConnect::RecvWithoutCallback( char* pData, int nLen, SocketConnect* pThis )
{
	return 0;
}

int ZUtil::SocketConnect::RecvWithCallback( char* pData, int nLen, SocketConnect* pThis )
{
	switch(nLen)
	{
	case -1:
		pThis->pNotifyFunc(ClientError, ClientErrorLen, ZSOCKET_ERROR, pThis);
		break;
	case 0:
		pThis->pNotifyFunc(ClientDisconnect, ClientDisconnectLen, ZSOCKET_DISCONNECTED, pThis);
		break;
	default:
		pThis->pNotifyFunc(pData, nLen, ZSOCKET_RECV, pThis);
		break;
	}
	return 0;
}

void ZUtil::SocketConnect::SetEncodeProtocolFunc( pProtocol pEncodeProtocolFunc )
{
	this->pEncodeProtocolFunc = pEncodeProtocolFunc;
	this->pSendFunc = CodeSend;
}

int ZUtil::SocketConnect::Send( char* pBuff, int nLen )
{
	return this->pSendFunc(pBuff, nLen, this);
}

void ZUtil::SocketConnect::DefaultCallback( char* pData, int nLen )
{
	//do nothing
}

int ZUtil::SocketConnect::RecvWithCallbackAndDecode( char* pData, int nLen, SocketConnect* pThis )
{
	char szOut[1024];
	int nOutLen = 0;
	pThis->pDecodeProtocolFunc(pData, nLen, szOut, nOutLen);
	switch(nLen)
	{
	case -1:
		pThis->pNotifyFunc(ClientError, ClientErrorLen, ZSOCKET_ERROR, pThis);
		break;
	case 0:
		pThis->pNotifyFunc(ClientDisconnect, ClientDisconnectLen, ZSOCKET_DISCONNECTED, pThis);
		break;
	default:
		pThis->pNotifyFunc(szOut, nOutLen, ZSOCKET_RECV, pThis);
		break;
	}
	return 0;
}

int ZUtil::SocketConnect::GetType()
{
	return nType;
}

void ZUtil::SocketConnect::SetPrea( SocketConnect* pPrea )
{
	this->pPrea = pPrea;
}

void ZUtil::SocketConnect::SetNext( SocketConnect* pNext )
{
	this->pNext = pNext;
}

ZUtil::SocketConnect* ZUtil::SocketConnect::GetNext()
{
	return pNext;
}

ZUtil::SocketConnect* ZUtil::SocketConnect::GetPrea()
{
	return pPrea;
}

void ZUtil::SocketConnect::SetSocket( unsigned int nSocket )
{
	this->nSocket = nSocket;
}

pNotify ZUtil::SocketConnect::GetNotify()
{
	return pNotifyFunc;
}

pSend ZUtil::SocketConnect::GetRecv()
{
	return pRecvFunc;
}

int ZUtil::SocketServer::InitConnectionParam( char* pIP, int nPort, pNotify pNotifyFunc )
{
	SocketConnect::InitConnectionParam(pIP, nPort, pNotifyFunc);
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(pIP);

	sockAddr.sin_port = htons(nPort);
	nSocket = socket(AF_INET, SOCK_STREAM, 0);
	int nNameLen = sizeof(sockAddr);
	if(bind(nSocket, (const sockaddr*)&sockAddr, nNameLen) != 0)//可能是端口被占用
	{
		closesocket(nSocket);
		return GetLastError();
	}
	listen(nSocket, 10);

	if (nPort == 0)//将打开随机端口
	{
		getsockname(nSocket, (sockaddr*)&sockAddr, &nNameLen);		
		this->nPort = htons(sockAddr.sin_port);//获取成功打开的随机端口
	}

	CreateThread(0, 0, OnAcceptThread, this, 0, 0);

	return 0;	
}

int ZUtil::SocketServer::Connect()
{
	return 0;
}

DWORD WINAPI ZUtil::SocketServer::OnAcceptThread( void* pThis )
{
	SOCKADDR_IN addrIn;
	int nLenName = sizeof(addrIn);
	SocketServer* This = (SocketServer*)pThis;
	while(1)
	{
		SOCKET remote = accept(This->GetSocket(), (sockaddr*)&addrIn, &nLenName);
		SocketConnect* pConnect = new SocketClient(SOCKET_CLIENT);
		pConnect->SetSocket(remote);
		pConnect->InitConnectionParam(inet_ntoa(addrIn.sin_addr), htons(addrIn.sin_port), This->pNotifyFunc);
		This->CheckEnCodeFunctions(pConnect);
		((SocketClient*)pConnect)->SetServer(This);
		CreateThread(0, 0, OnRecvThread, pConnect, 0, 0);
		if (This->pClients)
		{
			This->pLastClient->SetNext(pConnect);
			pConnect->SetPrea(This->pLastClient);
			This->pLastClient = pConnect;
		}
		else
		{
			This->pClients = pConnect;
			This->pLastClient = pConnect;
		}
	}
	return 0;
}

ZUtil::SocketServer::SocketServer( int nType ) : SocketConnect(nType)
{
	pClients = nullptr;
	pLastClient = nullptr;
}

ZUtil::SocketServer::~SocketServer()
{
	//remove and clear pClients
}

void ZUtil::SocketServer::CheckEnCodeFunctions( SocketConnect* pConnect )
{
	if (pDecodeProtocolFunc)
	{
		pConnect->SetDecodeProtocolFunc(pDecodeProtocolFunc);
	}
	if (pEncodeProtocolFunc)
	{
		pConnect->SetEncodeProtocolFunc(pEncodeProtocolFunc);
	}
}

DWORD WINAPI ZUtil::SocketServer::OnRecvThread( void* pThis )
{
	SocketConnect* This = (SocketConnect*)pThis;
	This->GetNotify()(ClientIn, ClientInLen, ZSOCKET_ACCEPT, This);
	char sz[1024];
	int nLen = 0;
	while(1){
		nLen = recv(This->GetSocket(), sz, 1023, 0);
		This->GetRecv()(sz, nLen, This);
		if (nLen <= 0)
		{
			break;
		}		
	}
	((SocketServer*)((SocketClient*)This)->GetServer())->RemoveClient(This);
	return 0;
}

void ZUtil::SocketServer::RemoveClient( SocketConnect* pClient )
{
	SocketConnect* pCurrent = pClients;
	if (pCurrent == pLastClient)//only one
	{
		delete pCurrent;
		pClients = pLastClient = nullptr;
		return;
	}

	bool bIsFirst = false;
	if (pClient == pClients)//first
	{
		bIsFirst = true;
	}
	bool bIsLast = false;
	if (pClient == pLastClient)
	{
		bIsLast = true;
	}

	for (; pCurrent != nullptr; pCurrent = pCurrent->GetNext())
	{
		if (pCurrent->GetSocket() == pClient->GetSocket())
		{
			if (bIsFirst)
			{
				pClients = pClients->GetNext();
			}
			if (bIsLast)
			{
				pLastClient = pLastClient->GetPrea();
			}			
			delete pCurrent;
			break;
		}
	}
	for (pCurrent = pClients; pCurrent != nullptr; pCurrent = pCurrent->GetNext())
	{
		pLastClient = pCurrent;
	}
}

ZUtil::SocketClient::SocketClient( int nType ) : SocketConnect(nType)
{

}

int ZUtil::SocketClient::Connect()
{
	return 0;
}

ZUtil::SocketClient::~SocketClient()
{

}

void ZUtil::SocketClient::SetServer( SocketConnect* pServer )
{
	this->pServer = pServer;
}

ZUtil::SocketConnect* ZUtil::SocketClient::GetServer()
{
	return pServer;
}

ZUtil::LockKey::LockKey()
{
	::InitializeCriticalSection(&cs);
}

ZUtil::LockKey::~LockKey()
{
	::DeleteCriticalSection(&cs);
}

CRITICAL_SECTION* ZUtil::LockKey::GetKey()
{
	return &cs;
}

ZUtil::Lock::Lock( LockKey* pKey )
{
	this->pKey = pKey;
	::EnterCriticalSection(pKey->GetKey());
}

ZUtil::Lock::~Lock()
{
	::LeaveCriticalSection(pKey->GetKey());
}

void ZUtil::FileManage::CreateDir( char* pDir )
{
	if (_chdir(pDir) == 0)
	{
		return;
	}

	char szBuff[1024] = "";
	strcpy(szBuff, pDir);
	char* pParentDir = strrchr(szBuff, '\\');
	if (pParentDir)
	{
		*pParentDir = 0;
		CreateDir(szBuff);
		_mkdir(szBuff);
	}
}

bool ZUtil::FileManage::RemoveDir( char* pDir )
{
	char szPath[1024] = "";
	strcat(szPath, pDir);
	strcat(szPath, "\\*.*");
	long lFile;
	char szAddPath[1024] = "";
	struct _finddata_t sFileInfo;

	lFile = _findfirst(szPath, &sFileInfo);
	if (lFile == -1)
	{
		remove(szPath);
		return false ;
	}
	
	while (_findnext(lFile, &sFileInfo) == 0)
	{

		sprintf(szAddPath, "%s\\%s", pDir, sFileInfo.name);
		if(sFileInfo.attrib == _A_SUBDIR)
		{
			if (strcmp(sFileInfo.name, ".") == 0 || strcmp(sFileInfo.name, "..") == 0)
			{
				continue;
			}
			RemoveDir(szAddPath);
		}
		else
		{
			remove(szAddPath);
		}
	}
	_findclose(lFile);
	// 删除文件夹
	rmdir(szAddPath);
	return true;
}

int ZUtil::FileManage::ReadFile( char* pFile, __out char* pRes, __in int nResLen )
{
	memset(pRes, 0, nResLen);
	if (_access(pFile, 0) == -1)
	{
		return -1;
	}

	FILE* pf = fopen(pFile, "r");
	fread(pRes, nResLen, 1, pf);
	fclose(pf);
	return 0;
}

int ZUtil::FileManage::WriteFile( char* pFileName, char* pData, int nLen, bool bAppend /*= true*/ )
{
	if (_access(pFileName, 0) == -1)
	{
		return -1;
	}

	FILE* pf = fopen(pFileName, bAppend ? "a" : "w");	
	fwrite(pData, nLen, 1, pf);
	fclose(pf);
	return 0;
}

int ZUtil::FileManage::DelFile( char* pFileName )
{
	return ::remove(pFileName);
}

int ZUtil::FileManage::MoveFile( char* pFileName, char* pNewFileName, bool bRemoveOld /*= false*/, bool bReWriteIsExist /*= false*/ )
{
	if (_access(pFileName, 0) == -1)
	{
		return FILE_NOT_EXIST;
	}
	if(_access(pNewFileName, 0) != -1)//exist
	{
		if ( ! bReWriteIsExist)
		{
			return FILE_EXIST;
		}
	}
	CopyFileA(pFileName, pNewFileName, false);

	if (bRemoveOld)
	{
		DelFile(pFileName);
	}
	return 0;
}

bool ZUtil::FileManage::CheckFileExist( char* pFile )
{
	if (_access(pFile, 0) == -1)
	{
		return false;
	}
	return true;
}

ZUtil::Thread::Thread()
{
	this->pParam = nullptr;
	this->pFunc = DefaultCallback;
	hThread = NULL;
}

ZUtil::Thread::Thread( void* pParam, pThreadEvent pFunc /*= nullptr*/ )
{
	this->pParam = pParam;
	this->pFunc = pFunc == nullptr ? DefaultCallback : pFunc;
	hThread = NULL;
}

void ZUtil::Thread::Start()
{
	if (hThread != nullptr)
	{
		pFunc("thread has existed, pls new a thread object to use", strlen("thread has created, pls new a thread object to use"), THREAD_ERROR);		
		return;
	}

	hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Work, this, 0, 0);
	pFunc("thread is start", strlen("thread is start"), THREAD_START);
}

void ZUtil::Thread::WaitForFinish()
{
	if (hThread)
	{
		WaitForSingleObject(hThread, INFINITE);
		pFunc("thread is finished", strlen("thread is finished"), THREAD_FINISH);
	}else{
		pFunc("no thread", strlen("no thread"), THREAD_ERROR);
	}
}

void ZUtil::Thread::Pause()
{
	if (hThread)
	{
		::SuspendThread(hThread);
		pFunc("thread is paused", strlen("thread is paused"), THREAD_PAUSE);
	}else{
		pFunc("no thread", strlen("no thread"), THREAD_ERROR);
	}
}

int ZUtil::Thread::DefaultCallback( char* pData, int nLen, int nEventType )
{
	printf("[%s][%d]\n", pData, nEventType);
	return 0;		 
}

void ZUtil::Thread::Resume()
{
	if (hThread)
	{
		::ResumeThread(hThread);
		pFunc("thread is resume", strlen("thread is resume"), THREAD_RESUME);
	}else{
		pFunc("no thread", strlen("no thread"), THREAD_ERROR);
	}
}

void ZUtil::Thread::Stop()
{
	if (hThread)
	{
		::TerminateThread(hThread, -1);
		pFunc("thread is stop", strlen("thread is stop"), THREAD_STOP);
	}else{
		pFunc("no thread", strlen("no thread"), THREAD_ERROR);
	}
}

DWORD WINAPI ZUtil::Thread::Work( void* pParam )
{
	Thread* pThread = (Thread*)pParam;
	int nRes = pThread->Run();
	pThread->hThread = nullptr;
	pThread->pFunc("thread is finish", strlen("thread is finish"), THREAD_FINISH);
	return nRes;
}

void ZUtil::Thread::SetThreadEventFunc( pThreadEvent pFunc )
{
	this->pFunc = pFunc;
}

ZUtil::Time::Time()
{
	this->ymdConnector = '-';
	this->hmsConnector = ':';
	this->fmt = yyyymmddhhmissms;
}

ZUtil::Time::Time( TimeFmt fmt, char ymdConnector /*= '-'*/, char hmsConnector /*= ':'*/ )
{
	this->ymdConnector = ymdConnector;
	this->hmsConnector = hmsConnector;
	this->fmt = fmt;
}

char* ZUtil::Time::GetTime()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	switch(fmt)
	{
	case yyyymmddhhmissms:
		sprintf(szTime, "%02d%c%02d%c%02d %02d%c%02d%c%02d.%04d", sysTime.wYear, ymdConnector, sysTime.wMonth, ymdConnector, sysTime.wDay, sysTime.wHour, hmsConnector, sysTime.wMinute, hmsConnector, sysTime.wSecond, sysTime.wMilliseconds);
		break;
	case yyyymmddhhmiss:
				sprintf(szTime, "%02d%c%02d%c%02d %02d%c%02d%c%02d", sysTime.wYear, ymdConnector, sysTime.wMonth, ymdConnector, sysTime.wDay, sysTime.wHour, hmsConnector, sysTime.wMinute, hmsConnector, sysTime.wSecond);
				break;
	case yyyymmdd:
		sprintf(szTime, "%02d%c%02d%c%02d", sysTime.wYear, ymdConnector, sysTime.wMonth, ymdConnector, sysTime.wDay);
		break;
	case hhmiss:
		sprintf(szTime, "%02d%c%02d%c%02d", sysTime.wHour, hmsConnector, sysTime.wMinute, hmsConnector, sysTime.wSecond);
		break;
	}
	return szTime;
}

int ZUtil::Time::operator-( Time& t2 )
{
	if (fmt != hhmiss && t2.GetFmt() == hhmiss){
		throw "can't getBalance";
		return 0;
	}
	return 1;
}

ZUtil::Time::TimeFmt ZUtil::Time::GetFmt()
{
	return fmt;
}

//
//ZUtil::AnnulusStore::AnnulusStore( unsigned int nSize )
//{
//	this->nBPos = this->nEPos = 0;
//	this->nMaxSize = nSize;
//	this->pData = new char[nSize];
//	memset(pData, 0, nSize);
//}
//
//ZUtil::AnnulusStore::~AnnulusStore()
//{
//	if (pData){
//		delete[] pData;
//	}
//}
//
//int ZUtil::AnnulusStore::GetData( char* pRes, int nLen )
//{
//	int nHead = 0;
//	int nTail = 0;
//	int nCopy = nLen > nMaxSize ? nMaxSize : nLen;//可接收部分
//	if (nBPos <= nEPos){//顺序
//		nCopy = (nEPos - nHead) > nCopy ? nCopy : (nEPos - nHead);
//		memcpy(pRes, &pData[nBPos], nCopy);
//		return (nCopy - nLen);
//	}else{//翻过尾拼头
//		int nPos = nMaxSize - nBPos;//尾部
//		if (nPos > nCopy){//接收区不够长
//			memcpy(pRes, &pData[nBPos], nCopy);
//			return (nPos - nCopy);
//		}else{//够长
//			memcpy(pRes, &pData[nBPos], nPos);
//			if (nLen - nPos < nBPos){//剩下的不够放
//				memcpy(&pRes[nPos], &pData[0], (nLen - nPos));
//				return (nBPos - (nLen - nPos));
//			}
//			memcpy(&pRes[nPos], &pData[0], nEPos);
//			return 0;
//		}
//	}		
//}
//
//int ZUtil::AnnulusStore::WriteData( char* pData, int nLen )
//{
//	int nCopyLen = 0;
//	int nCopyTotal = 0;
//	while(nLen > 0){
//		nCopyLen = nMaxSize - nEPos;
//		memcpy(&pData[nCopyTotal], &this->pData[nBPos], nCopyLen);
//		nCopyTotal += nCopyLen;
//		nEPos+= nCopyLen;
//	}
//	return 0;
//}

ZUtil::string::string( std::string& strSrc )
{
	this->strSrc = strSrc;
}

ZUtil::string::string(char* pSrc, int nLen)
{
	this->strSrc = std::string(pSrc, nLen);
}

ZUtil::string::string(char* pSrc)
{
	this->strSrc = std::string(pSrc);
}

ZUtil::string::~string()
{
	printf("%s, %p\n", __FUNCTION__, this);
}

void ZUtil::string::Reset()
{
	this->strSrc = "";
	this->strSrc.resize(0);
}

std::string ZUtil::string::CopyStdString()
{
	return strSrc;
}

int ZUtil::string::Number()
{
	return atoi(strSrc.c_str());
}

std::string* ZUtil::string::GetStdString()
{
	return &strSrc;
}

ZUtil::string& ZUtil::string::Lower()
{
	for (int i = 0; i < strSrc.length(); i++){
		if (strSrc.at(i) >= 0x41 && strSrc.at(i) <= 0x5A){
			strSrc.at(i) += 0x20;
		}
	}
	return *this;
}

ZUtil::string& ZUtil::string::Upper()
{
	for (int i = 0; i < strSrc.length(); i++){
		if (strSrc.at(i) >= 0x61 && strSrc.at(i) <= 0x7A){
			strSrc.at(i) -= 0x20;
		}
	}
	return *this;
}

int ZUtil::string::Length()
{
	return strSrc.length();
}

int ZUtil::string::Size()
{
	return strSrc.size();
}



bool ZUtil::string::Split( char* pSplit, std::vector<std::string>& vec )
{
	//int nSplitLen = strlen(pSplit);
	//char* pSrc = new char[strSrc.length()];
	//strcpy(&pSrc[0], strSrc.c_str());
	//char* pText = strtok(pSrc, pSplit);
	//bool bRes = false;

	//while(pText != nullptr){
	//	bRes = true;
	//	vec.push_back(pText);
	//	pText = strtok(nullptr, pSplit);
	//}
	//if (pText){
	//	vec.push_back(pText);
	//}
	//return bRes;
	if (pSplit == nullptr){
		vec.push_back(strSrc);
		return true;
	}
	std::string strSrc = this->strSrc;
	int nPos = strSrc.find(pSplit);
	if (nPos == -1){
		return false;
	}
	int nLen = strlen(pSplit);
	std::string strTemp;
	while(nPos != -1){
		while (nPos == 0){
			strSrc.erase(0, nLen);
			nPos = strSrc.find(pSplit);
		}
		strTemp = "";
		strTemp = strSrc.substr(0, nPos);
		vec.push_back(strTemp);
		strSrc.erase(0, nPos + nLen);
		nPos = strSrc.find(pSplit);
	}

	if ( ! strSrc.empty()){
		vec.push_back(strSrc);
	}
	return true;
}


ZUtil::stringex::stringex() : std::string()
{

}

ZUtil::stringex::stringex(char* pSrc, int nLen) : std::string(pSrc, nLen){

}

ZUtil::stringex::stringex(char* pSrc) : std::string(pSrc){

}

ZUtil::stringex::stringex(int value) : std::string(""){
	char sz[128] = "";
	sprintf(sz, "%d", value);
	this->append(sz);
}

ZUtil::stringex::stringex(const stringex& str) : std::string(""){	
	this->append(str);
}

ZUtil::stringex::~stringex()
{

}

ZUtil::stringex& ZUtil::stringex::Lower()
{
	for (size_t i = 0; i < length(); i++){
		if (at(i) >= 0x41 && at(i) <= 0x5A){
			at(i) += 0x20;
		}
	}
	return *this;
}

ZUtil::stringex& ZUtil::stringex::Upper()
{
	for (size_t i = 0; i < length(); i++){
		if (at(i) >= 0x61 && at(i) <= 0x7A){
			at(i) -= 0x20;
		}
	}
	return *this;
}

int ZUtil::stringex::Number()
{
	return atoi(c_str());
}

double ZUtil::stringex::DoubleNumber()
{
	return atof(c_str());
}

bool ZUtil::stringex::Split( char* pSplit, std::vector<stringex>& vec )
{
	if (pSplit == nullptr){
		vec.push_back(*this);
		return true;
	}

	ZUtil::stringex strSrc = *this;
	int nPos = strSrc.find(pSplit);
	if (nPos == -1){
		return false;
	}
	int nLen = strlen(pSplit);
	stringex strTemp;
	while(nPos != -1){
		while (nPos == 0){
			strSrc.erase(0, nLen);
			nPos = strSrc.find(pSplit);
		}
		strTemp = "";
		strTemp = ZUtil::stringex((char*)strSrc.substr(0, nPos).c_str());
		vec.push_back(strTemp);
		strSrc.erase(0, nPos + nLen);
		nPos = strSrc.find(pSplit);
	}

	if ( ! strSrc.empty()){
		vec.push_back(strSrc);
	}
	return true;
}

ZUtil::ServiceManage::ServiceManage() : hServiceMgr(NULL)
{
	int nCount = 0;
	while (hServiceMgr == NULL){
		hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (++nCount > 10){
			break;
		}
	}	
}

ZUtil::ServiceManage::~ServiceManage()
{
	if (hServiceMgr){
		CloseServiceHandle(hServiceMgr);
	}
}

int ZUtil::ServiceManage::RunService( const char* pServiceName )
{
	bool bSuss = false;
	do 
	{
		SC_HANDLE hService = OpenServiceA(hServiceMgr, pServiceName, SERVICE_ALL_ACCESS);
		if (nullptr == hService)
		{
			break;
		}
		SERVICE_STATUS status;
		if(QueryServiceStatus(hService, &status) == TRUE && SERVICE_RUNNING != status.dwCurrentState)
		{
			if(::StartService(hService, 0, NULL) == FALSE)
			{
				break;
			}
			bSuss = true;
		}
		else
		{
			break;
		}
		CloseServiceHandle(hService);
	} while (false);

	return bSuss ? 0 : GetLastError();
}

int ZUtil::ServiceManage::StopService( const char* pServiceName )
{
	bool bSuss = false;
	do 
	{
		int nTryTimes = 5;
		SC_HANDLE hService = OpenServiceA(hServiceMgr, pServiceName, SERVICE_ALL_ACCESS);
		if (nullptr == hService)
		{
			break;
		}
		SERVICE_STATUS status;
		if(QueryServiceStatus(hService, &status) == TRUE && SERVICE_RUNNING == status.dwCurrentState)
		{
			if(ControlService(hService, SERVICE_CONTROL_STOP, &status) == TRUE)
			{
				Sleep(status.dwWaitHint);
				while(QueryServiceStatus(hService, &status) == TRUE)
				{					
					if(SERVICE_STOPPED == status.dwCurrentState || nTryTimes <= 0)
					{
						bSuss = true;
						break;
					}
					else
					{
						Sleep(status.dwWaitHint);
						nTryTimes--;
					}
				}
			}
		}		
		CloseServiceHandle(hService);
	} while (false);

	return bSuss ? 0 : GetLastError();
}

int ZUtil::ServiceManage::CreateKernelService( const char* pPath, const char* pName, const char* pDisplayName )
{
	if(CreateServiceA(hServiceMgr, pName, pDisplayName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_SYSTEM_START, SERVICE_ERROR_NORMAL, pPath, NULL, NULL, NULL, NULL, NULL) == nullptr)
		return GetLastError();
	return 0;
}

int ZUtil::ServiceManage::DeleteService( const char* pName )
{
	bool bSuss = false;
	SC_HANDLE hService = nullptr;
	do 
	{
		StopService(pName);
		hService = OpenServiceA(hServiceMgr, pName, SERVICE_ALL_ACCESS | DELETE);
		if (hService == nullptr)
		{
			break;
		}

		if (::DeleteService(hService))
		{
			bSuss = true;
		}

	} while (false);
	if (hService)
	{
		CloseServiceHandle(hService);
	}
	return bSuss ? 0 : GetLastError();
}

ZUtil::FindFile::FileInfo::FileInfo( std::string strFileName, std::string strFilePath, std::string strExt, bool bIsDir )
{
	this->strFileName = strFileName;
	this->strFilePath = strFilePath;
	this->strExt = strExt;
	this->bIsDir = bIsDir;
}

ZUtil::FindFile::FileInfo::FileInfo( std::string strFilePath, std::string strName, bool bIsDir )
{
	SetInfo(strFilePath, strName, bIsDir);
}

std::string ZUtil::FindFile::FileInfo::GetName()
{
	return strFileName;
}

std::string ZUtil::FindFile::FileInfo::GetPath()
{
	return strFilePath;
}

std::string ZUtil::FindFile::FileInfo::GetExt()
{
	return strExt;
}

bool ZUtil::FindFile::FileInfo::IsDir()
{
	return bIsDir;
}

void ZUtil::FindFile::FileInfo::SetInfo( std::string strFilePath, std::string strName, bool bIsDir )
{
	this->strFilePath = strFilePath;
	int nPos = strFilePath.find('*');
	if (nPos != -1){
		this->strFilePath = strFilePath.substr(0, nPos);
	}else{
		this->strFilePath = strFilePath;
	}

	this->bIsDir = bIsDir;

	nPos = strName.rfind('.');
	if (nPos == -1){
		this->strFileName = strName;		
	}else{
		if (bIsDir){//is dir
			this->strFileName = strName;		
			return;
		}
		this->strFileName = strName.substr(0, nPos);
		strName.erase(0, nPos + 1);
		this->strExt = strName;
	}
}

ZUtil::FindFile::FindFile( char* pRoot ) : strRoot(pRoot)	
{
	ZUtil::Time time1;
	printf("\n[time: %s]", time1.GetTime());
	Run();
	ZUtil::Time time2;
	printf("\n[time: %s] size = %d", time2.GetTime(), _map.size());

	for (std::map<std::string, FileInfo>::iterator iter = _map.begin(); iter != _map.end(); iter++){
		printf("Path: %s%s\n", iter->first.c_str(), iter->second.IsDir() == true ? "<dir>" : "");
	}
}

std::map<std::string, ZUtil::FindFile::FileInfo>* ZUtil::FindFile::GetMap()
{
	return &_map;
}

std::string ZUtil::FindFile::GetRoot()
{
	return strRoot;
}

void ZUtil::FindFile::Run()
{
	list<FileInfo> listTemp, total;
	int nSize = Find((char*)strRoot.c_str(), listTemp);

	while(listTemp.size() > 0){
		list<FileInfo>::iterator iter = listTemp.begin();
		for (; iter != listTemp.end(); ){			
			if (iter->IsDir() == false){
				std::string strPath(iter->GetPath() + iter->GetName() + "." + iter->GetExt());
				_map.insert(make_pair(strPath, *iter));
				iter = listTemp.erase(iter);//only dir left
			}else{
				std::string strPath(iter->GetPath() + iter->GetName());
				_map.insert(make_pair(strPath, *iter));
				iter++;
			}		
		}
		if (listTemp.size() <= 0){
			break;
		}
		Find((char*)std::string(listTemp.front().GetPath() + listTemp.front().GetName() + "\\*.*").c_str(), listTemp);
		listTemp.pop_front();
	}
}

int ZUtil::FindFile::Find( char* pParent, std::list<FileInfo>& vec )
{
	WIN32_FIND_DATAA fileData;
	HANDLE hFile = FindFirstFileA(pParent, &fileData);
	if (hFile == INVALID_HANDLE_VALUE){
		return 0;
	}

	FileInfo fileInfo(pParent, fileData.cFileName, fileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY);
	if (strcmp(fileData.cFileName, ".") != 0 && strcmp(fileData.cFileName, "..") != 0 && fileData.cFileName == nullptr){
		vec.push_back(fileInfo);
	}

	while(FindNextFileA(hFile, &fileData)){
		if(fileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			if (strcmp(fileData.cFileName, ".") == 0 || strcmp(fileData.cFileName, "..") == 0 || fileData.cFileName == nullptr){
				continue;
			}
			fileInfo.SetInfo(pParent, fileData.cFileName, true);
			vec.push_back(fileInfo);
			continue;
		}
		fileInfo.SetInfo(pParent, fileData.cFileName, false);
		vec.push_back(fileInfo);
	}
	FindClose(hFile);
	return vec.size();
}

ZUtil::FileAttribute::FileAttribute( char* pFileName )
{
	this->strName = pFileName;
	szAcceseTime[0] = 0;
	szCreateTime[0] = 0;
	szWriteTime[0] = 0;
	lSize = 0;

	WIN32_FIND_DATAA fileData;
	HANDLE hFile = FindFirstFileA(pFileName, &fileData);
	if (hFile != INVALID_HANDLE_VALUE){
		SYSTEMTIME sysTime;
		FILETIME ft;
		FileTimeToLocalFileTime(&fileData.ftCreationTime, &ft);
		FileTimeToSystemTime(&ft, &sysTime);
		sprintf(szCreateTime, "%04d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		FileTimeToLocalFileTime(&fileData.ftLastAccessTime, &ft);
		FileTimeToSystemTime(&ft, &sysTime);
		sprintf(szAcceseTime, "%04d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);		

		FileTimeToLocalFileTime(&fileData.ftLastWriteTime, &ft);
		FileTimeToSystemTime(&ft, &sysTime);
		sprintf(szWriteTime, "%04d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		lSize = fileData.nFileSizeLow;
		std::string strTemp = strName;
		
		int nPos = strTemp.rfind('.');
		if (nPos != -1){
			strExt = strTemp.substr(nPos + 1, strTemp.npos);
		}

		nPos = strTemp.rfind('\\');
		if (nPos != -1){
			strPath = strTemp.substr(0, nPos);
		}

		if (nPos != -1){
			strTemp.erase(0, nPos + 1);
			strName = strTemp;
		}

		bIsDir = (fileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY);
	}	
}

long ZUtil::FileAttribute::Size()
{
	return lSize;
}

std::string ZUtil::FileAttribute::CreateTime()
{
	return szCreateTime;
}

std::string ZUtil::FileAttribute::AcceseTime()
{
	return szAcceseTime;
}

std::string ZUtil::FileAttribute::WriteTime()
{
	return szWriteTime;
}

std::string ZUtil::FileAttribute::Ext()
{
	return strExt;
}

std::string ZUtil::FileAttribute::Path()
{
	return strPath;
}

std::string ZUtil::FileAttribute::FileName()
{
	return strName;
}

bool ZUtil::FileAttribute::IsDir()
{
	return bIsDir;
}
