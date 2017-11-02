#include "NekoIOCP.h"

#define _CRT_SECURE_NO_WARNINGS 
#include <string.h>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MaxDataBuffSize 1024


using namespace Neko;

struct ServerInfo{
#define IP_LEN 15
	char szIP[IP_LEN];
	int nPort = 0;
	pfnOnCallback pCallback = 0;
	HANDLE hIOCP = INVALID_HANDLE_VALUE;
	HANDLE hAccept = INVALID_HANDLE_VALUE;
	HANDLE hEvent = INVALID_HANDLE_VALUE;
	WSAData wsaData;
	SOCKET s;
	int nRecvThreads = 0;
	bool bRunning = true;
	ServerInfo(){
		memset(szIP, 0, IP_LEN);
		WSAStartup(0x0202, &wsaData);
		s = socket(AF_INET, SOCK_STREAM, 0);
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
};

static ServerInfo serverInfo;

class IoOperationData{
public:
	
	OVERLAPPED overlapped;
	WSABUF databuff;
	char szBuff[MaxDataBuffSize];
	int nLen;
	int nOperationType;
	void Reset(){
		memset(&overlapped, 0, sizeof(overlapped));
		this->databuff.buf = this->szBuff;
		this->databuff.len = 1024;
		this->nOperationType = 0;
	};
	IoOperationData(){
		Reset();
	};
};

std::string MakeString(const char* pFmt, ...){
	char sz[MaxDataBuffSize * 5] = "";
	va_list ap;
	va_start(ap, pFmt);
	vsprintf(sz, pFmt, ap);
	va_end(ap);
	return std::string(sz);
}

void AcceptThread(ServerInfo* pInfo){
	SOCKADDR_IN sockAddr;
	int nLen = sizeof(sockAddr);
	SOCKET* pClient = nullptr;
	while (pInfo->bRunning){
		pClient = new SOCKET;
		*pClient = accept(pInfo->s, (sockaddr*)&sockAddr, &nLen);
		printf("AcceptThread[%d]", GetCurrentThreadId());
		pInfo->pCallback(*pClient, 0, MakeString("%s:%d", inet_ntoa(sockAddr.sin_addr), ntohs(sockAddr.sin_port)).c_str(), _IOCP_CONNECT);
		CreateIoCompletionPort((HANDLE)*pClient, pInfo->hIOCP, (ULONG_PTR)pClient, 0);
		IoOperationData* pIoData = nullptr;
		pIoData = new IoOperationData;
		DWORD dwRecvBytes = 0, dwFlags = 0;
		WSARecv(*pClient, &pIoData->databuff, 1, &dwRecvBytes, &dwFlags, &pIoData->overlapped, NULL);
	}

	pInfo->pCallback(0, 0, "Recv Close IOCP", _IOCP_DISCONNECT);
	closesocket(*pClient);
	delete pClient;
	SetEvent(pInfo->hEvent);
	return;
}

void RecvThread(ServerInfo* pInfo){
	DWORD dwReadBytes = 0, dwFlags = 0;
	IoOperationData* lpIoOperationData = nullptr;
	LPOVERLAPPED lpOverlapped;
	SOCKET* pClient = nullptr;
	while (pInfo->bRunning){
		if (GetQueuedCompletionStatus(pInfo->hIOCP, &dwReadBytes, (PULONG_PTR)&pClient, (LPOVERLAPPED*)&lpOverlapped, INFINITE) == 0){
			//error socket disconnect not standardly
			dwReadBytes = -1;
		}
		lpIoOperationData = (IoOperationData*)CONTAINING_RECORD(lpOverlapped, IoOperationData, overlapped);
		printf("RecvThread[%d]", GetCurrentThreadId());
		if (pClient == 0){
			pInfo->pCallback(0, 0, "Recv close IOCP", _IOCP_DISCONNECT);
			if (lpIoOperationData){
				delete lpIoOperationData;
			}
			return;
		}
		
		if (dwReadBytes <= 0){//socket disconnect and error			
			pInfo->pCallback(*pClient, dwReadBytes, dwReadBytes == 0 ? MakeString("Disconnect:%d", *pClient).c_str() : MakeString("ErrorConnect:%d", *pClient).c_str(), dwReadBytes == 0 ? _IOCP_DISCONNECT : _IOCP_ERROR);			
			closesocket(*pClient);			
			delete pClient;
			delete lpIoOperationData;
			continue;
		}
		lpIoOperationData->nLen = dwReadBytes;
		lpIoOperationData->szBuff[dwReadBytes] = 0;
		pInfo->pCallback(*pClient, dwReadBytes, lpIoOperationData->szBuff, _IOCP_RECV);		
		lpIoOperationData->Reset();
		WSARecv(*pClient, &lpIoOperationData->databuff, 1, &dwReadBytes, &dwFlags, &lpIoOperationData->overlapped, NULL);
	}
}

void* Neko::CreateIOCP(pfnOnCallback pCallback, const char* pIP /*= "0.0.0.0"*/, int nPort /*= 0*/)
{
	serverInfo.pCallback = pCallback;
	strcpy(serverInfo.szIP, pIP);
	serverInfo.hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(pIP);
	sockAddr.sin_port = htons(nPort);
	if (bind(serverInfo.s, (sockaddr*)&sockAddr, sizeof(sockAddr)) != 0){		
		pCallback(0, 0, MakeString("bind fail, error code is [%d]", WSAGetLastError()).c_str(), _IOCP_ERROR);
		return NULL;
	}

	if (nPort == 0){
		SOCKADDR_IN sockAddr;
		int nLen = sizeof(sockAddr);
		if (getsockname(serverInfo.s, (sockaddr*)&sockAddr, &nLen) != 0){
			pCallback(0, 0, MakeString("get random port fail, error code is [%d]", WSAGetLastError()).c_str(), _IOCP_ERROR);
			return NULL;
		}
		nPort = ntohs(sockAddr.sin_port);
	}
	serverInfo.nPort = nPort;
	printf("running on port:%d\n", nPort);
	if (listen(serverInfo.s, 10) != 0){
		pCallback(0, 0, MakeString("listen fail, error code is [%s]", WSAGetLastError()).c_str(), _IOCP_ERROR);
		return NULL;
	}

	serverInfo.hAccept = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AcceptThread, &serverInfo, 0, 0);
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	for (int i = 0; i < sysInfo.dwNumberOfProcessors; i++){
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RecvThread, &serverInfo, 0, 0);
	}
	serverInfo.nRecvThreads = sysInfo.dwNumberOfProcessors;

	Sleep(2000);
	CloseIOCP();
	return &serverInfo;
}

int Neko::GetPort()
{
	return serverInfo.nPort;
}

void Neko::CloseIOCP()
{
	SOCKET sClose = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(serverInfo.szIP);
	sockAddr.sin_port = htons(serverInfo.nPort);
	serverInfo.bRunning = false;
	if (0 == connect(sClose, (sockaddr*)&sockAddr, sizeof(sockAddr))){
		for (int i = 0; i < serverInfo.nRecvThreads; i++){
			PostQueuedCompletionStatus(serverInfo.hIOCP, 0, 0, 0);
		}		
		WaitForSingleObject(serverInfo.hEvent, 5000);
		shutdown(serverInfo.s, SD_BOTH);
		return;
	};	
}
