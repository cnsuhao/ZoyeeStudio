#include "stdafx.h"
#include "MD5head.h"
#include <iostream>
#include "md5.h"


MD5_API void md5( char* pInput, int nInputLen, __out char* pOutput32 )
{
	MD5_CTX mdt;
	unsigned char sz[16] = "";
	MD5Init(&mdt);
	MD5Update(&mdt, (unsigned char*)pInput, nInputLen);
	MD5Final(&mdt, sz);
	pOutput32[0] = 0;
	char szTemp[4];
	for (int i = 0; i < 16; i++){
		sprintf(szTemp, "%02x", sz[i]);
		strcat(pOutput32, szTemp);
	}
}
