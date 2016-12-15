// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 APIHOOK_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// APIHOOK_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef APIHOOK_EXPORTS
#define APIHOOK_API __declspec(dllexport)
#else
#define APIHOOK_API __declspec(dllimport)
#endif
#include <WinSock2.h>
#include <Windows.h>
//
//// 此类是从 ApiHook.dll 导出的
//class APIHOOK_API CApiHook {
//public:
//	CApiHook(void);
//	// TODO: 在此添加您的方法。
//};
//
//extern APIHOOK_API int nApiHook;
//
//APIHOOK_API int fnApiHook(void);

typedef HANDLE (WINAPI* pfnCreateFileA)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE (WINAPI* pfnCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

typedef int (WINAPI* pfnBind)(SOCKET s, const struct sockaddr FAR *addr, int namelen);
typedef int (WINAPI* pfnConnect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
typedef int (WINAPI* pfnRecv)(SOCKET s, char* buf, int len, int flags);
typedef int (WINAPI* pfnSend)(SOCKET s, char* buf, int len, int flags);

typedef LSTATUS (WINAPI* pfnRegOpenKeyExA)(HKEY hKey, LPCSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);
typedef LSTATUS (WINAPI* pfnRegOpenKeyExW)(HKEY hKey, LPCWSTR lpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);
typedef LSTATUS (WINAPI* pfnRegSetValueExA)(HKEY hKey, LPCSTR lpValueName, DWORD dwReserved, DWORD dwType, CONST BYTE* lpData, DWORD dwData);
typedef LSTATUS (WINAPI* pfnRegSetValueExW)(HKEY hKey, LPCWSTR lpValueName, DWORD dwReserved, DWORD dwType, CONST BYTE* lpData, DWORD dwData);

extern "C" APIHOOK_API void SetAPIHook(void* pTrueFunc, void* pFakeFunc);
extern "C" APIHOOK_API void SetAPIUnHook(void* pTrueFunc);
extern "C" APIHOOK_API void Test();