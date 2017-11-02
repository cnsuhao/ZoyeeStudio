#include "NekoIOCP.h"
#include <windows.h>
#include <iostream>
void defaultOnCallback(int nSocket, const int nLen, const char* pBuffer, int nType){
	printf("type:%d, socket:%d, buffer:%s\n", nType, nSocket, pBuffer);
	return;
}
#if LIB_DEBUG 
int __2C530BAA496146979D2BB060F27554FA(int argc, char** argv)
#elif LIB_RELEASE 
int __2C530BAA496146979D2BB060F27554FA(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
	Neko::CreateIOCP(defaultOnCallback, "127.0.0.1", 5542);
	while (1){
		Sleep(10);
	}
	return 0;
}