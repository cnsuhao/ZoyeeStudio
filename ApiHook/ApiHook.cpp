// ApiHook.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ApiHook.h"
#include "mhook.h"

HMODULE hDll = LoadLibraryA("Advapi32.dll");
pfnRegOpenKeyExA pFun = (pfnRegOpenKeyExA)GetProcAddress(hDll, "RegOpenKeyExA");

extern "C" APIHOOK_API void SetAPIHook( void* pTrueFunc, void* pFakeFunc )
{
	Mhook_SetHook((PVOID*)&pTrueFunc, (void*)pFakeFunc);
}

extern "C" APIHOOK_API void SetAPIUnHook( void* pTrueFunc)
{
	Mhook_Unhook((PVOID*)&pTrueFunc);
}


LSTATUS WINAPI FakeRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult){
	return pFun(hKey, lpSubKey, dwOptions, samDesired, phkResult);
}

extern "C" APIHOOK_API void Test()
{
	Mhook_SetHook((PVOID*)&pFun, FakeRegOpenKeyExA);
}
