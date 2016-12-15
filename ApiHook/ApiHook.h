// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� APIHOOK_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// APIHOOK_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef APIHOOK_EXPORTS
#define APIHOOK_API __declspec(dllexport)
#else
#define APIHOOK_API __declspec(dllimport)
#endif
#include <WinSock2.h>
#include <Windows.h>
//
//// �����Ǵ� ApiHook.dll ������
//class APIHOOK_API CApiHook {
//public:
//	CApiHook(void);
//	// TODO: �ڴ�������ķ�����
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