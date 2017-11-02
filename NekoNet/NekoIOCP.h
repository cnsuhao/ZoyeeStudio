#ifndef __I_NEKOIOCP_H__
#define __I_NEKOIOCP_H__

#define _IOCP_RECV 0
#define _IOCP_CONNECT 1
#define _IOCP_DISCONNECT 2
#define _IOCP_ERROR 3

typedef void(*pfnOnCallback)(int nSocket, const int nLen, const char* pBuffer, int nType);

namespace Neko{
	void* CreateIOCP(pfnOnCallback pCallback, const char* pIP = "127.0.0.1", int nPort = 0);
	int GetPort();
	void CloseIOCP();
}

#endif