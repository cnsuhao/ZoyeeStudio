#include "../ApiHook/ApiHook.h"

typedef void(*pfnSetAPIHook)(void* pTrue, void* pFake);
typedef LSTATUS  (WINAPI*pfnRegOpenKeyExA)(HKEY hKey, LPCSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);

HMODULE hKerel32 = LoadLibraryA("kernel32.dll");
pfnRegOpenKeyExA pfn = (pfnRegOpenKeyExA)GetProcAddress(hKerel32, "RegOpenKeyExA");

LSTATUS WINAPI FakeRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult){
	return pfn(hKey, lpSubKey, dwOptions, samDesired, phkResult);
}

typedef void(*pTest)();


int main(){
	HMODULE hModule = LoadLibraryA("../output/debug/apihook.dll");
	pfnSetAPIHook _init = (pfnSetAPIHook)GetProcAddress(hModule, "SetAPIHook");
	pTest _pFun = (pTest)GetProcAddress(hModule, "Test");
	_pFun();
	//_init(RegOpenKeyExA, FakeRegOpenKeyExA);
	HKEY hKey;	
	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "", 0, 0, &hKey);
	return 0;
}