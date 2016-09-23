#include "stdafx.h"
#include "ZipHead.h"

#include "zip.h"
#include "unzip.h"

const int nMaxLen = 260;

void ToChar( wchar_t* pwData, int nwLen, __out char* pData )
{
	WideCharToMultiByte(CP_ACP, 0, pwData, -1, pData, (nwLen * 2), nullptr, nullptr);
}

void ToWchar( char* pData, int nLen, __out wchar_t* pwData )
{
	MultiByteToWideChar(CP_ACP, 0, pData, -1, pwData, (nLen * 2));
}

#define ToW(sz, tsz) TCHAR tsz[nMaxLen]; ToWchar(sz, strlen(sz), tsz);

ZIP_API void* CreateZipFile( char* pZipFile, char* pPassword /*= nullptr*/)
{
	ToW(pZipFile, szZipFile);
	HZIP hZip = CreateZip(szZipFile, pPassword);
	return hZip;
}

ZIP_API void* OpenZipFile( char* pZipFile, char* pPassword /*= nullptr*/)
{
	ToW(pZipFile, szZipFile);
	HZIP hZip = OpenZip(szZipFile, pPassword);
	return hZip;
}

ZIP_API void CloseZipFile( void* pZip )
{
	CloseZip((HZIP)pZip);
}

ZIP_API int Zip( void* pZip, char* pFromZipFilePath, char* pInZipFilePath, bool bIsDir /*= false*/ )
{
	ToW(pFromZipFilePath, szFromZipFilePath);
	ToW(pInZipFilePath, szInZipFilePath);
	if (bIsDir){
		return ZipAddFolder((HZIP)pZip, szInZipFilePath);
	}else{
		return ZipAdd((HZIP)pZip, szFromZipFilePath, szInZipFilePath);
	}	
}

ZIP_API int GetUnZipFileCount( void* pZip )
{
	ZIPENTRY ze;
	GetZipItem((HZIP)pZip, -1, &ze);
	return ze.index;
}

ZIP_API int GetUnZipFile(void* pZip, int nIndex, char* pFileName)
{
	ZIPENTRY ze;
	ZRESULT zresult = GetZipItem((HZIP)pZip, nIndex, &ze);
	if(zresult == ZR_OK){
		ToChar(ze.name, wcslen(ze.name), pFileName);
	}
	return zresult;
}

ZIP_API int UnZip( void* pZip, int nIndex )
{
	ZIPENTRY ze;
	ZRESULT zresult = GetZipItem((HZIP)pZip, nIndex, &ze);
	if (zresult == ZR_OK){
		return UnzipItem((HZIP)pZip, nIndex, ze.name);
	}
	return zresult;
}

ZIP_API int SetUnZipDir( void* pZip, char* pDir )
{
	ToW(pDir, szDir);
	SetUnzipBaseDir((HZIP)pZip, szDir);
}
