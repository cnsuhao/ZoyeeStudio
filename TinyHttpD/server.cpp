#include "server.h"
#include <iostream>
#include <Windows.h>
#include <sys/stat.h>
#pragma comment(lib, "ws2_32.lib")

#include "TinyHttpDExport.h"

#define IsSpace(x) (x == 0x20)
#define MaxBuffLen 1024
#define MaxPathLen 512
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"

static char szAppPath[1024] = "";

int TinyHttpD::GetLine( unsigned int nSocket, char* pBuffer, int nBuffSize )
{
	int i = 0;
	char c[1];
	*c = 0;
	int n;
	memset(pBuffer, 0, nBuffSize);
	//n = recv(nSocket, pBuffer, 1024, 0);
	//pBuffer[n] = 0;
	while(1){
		n = recv(nSocket, c, 1, 0);
		if (*c == '\n' || i > nBuffSize - 1){
			break;
		}
		pBuffer[i] = *c;
		i++;
	}
/*
	while((i < nBuffSize - 1) && (*c != '\n')){
		n = recv(nSocket, c, 1, 0);
		if (n > 0){
			if (*c == '\r'){
				n = recv(nSocket, c, 1, MSG_PEEK);
				printf("%02x", c[0]);
				if ((n > 0) && (*c == '\n')){
					recv(nSocket, c, 1, 0);
				}else{
					*c = '\n';
				}
				pBuffer[i] = *c;
				i++;
			}			
		}else{
			*c = '\n';
		}
	}
	pBuffer[i] = 0;*/
	return i;
}

void TinyHttpD::Unimplemented( unsigned int nSocket )
{
	char szBuff[MaxBuffLen];
	sprintf(szBuff, "HTTP/1.0 501 Method Not Implemented\r\n"
		SERVER_STRING
		"Content-Type: text/html\r\n"
		"\r\n"
		"<HTML><HEAD><TITLE>Method Not Implemented\r\n"
		"</TITLE></HEAD>\r\n"
		"<BODY><P>HTTP request method not supported.\r\n"
		"</BODY></HTML>\r\n");
	send(nSocket, szBuff, strlen(szBuff), 0);
}

void TinyHttpD::NotFound( unsigned int nSocket )
{
	char szBuff[MaxBuffLen];
	sprintf(szBuff, "HTTP/1.0 404 NOT FOUND\r\n"
		SERVER_STRING
		"Content-Type: text/html\r\n"
		"\r\n"
		"<HTML><TITLE>Not Found</TITLE>\r\n"
		"<BODY><P>The server could not fulfill\r\n"
		"your request because the resource specified\r\n"
		"is unavailable or nonexistent.\r\n"
		"</BODY></HTML>\r\n");
	send(nSocket, szBuff, strlen(szBuff), 0);
}

void TinyHttpD::ServerFile( unsigned int nSocket, char* pPath )
{
	FILE* pResource = nullptr;
	int nNumChar = 1;
	char szBuff[MaxBuffLen];
	szBuff[0] = 'A';
	szBuff[1] = '\0';
	//while((nNumChar > 0) && strcmp("\n", szBuff))
	//	nNumChar = GetLine(nSocket, szBuff, MaxBuffLen);
	pResource = fopen(pPath, "r");
	if (pResource == nullptr){
		NotFound(nSocket);
	}else{
		SendHeader(nSocket, pPath);
		Cat(nSocket, pResource);
	}
	fclose(pResource);
}

void TinyHttpD::ExecuteCgi( unsigned int nSocket, char* pPath, char* pMethod, char* pQueryString )
{
	//todo
	char szBuff[MaxBuffLen];
	int cgiOutput[2];
	int cgiInput[2];

}

void TinyHttpD::Close( unsigned int nSocket )
{

}

void TinyHttpD::DealRequest( unsigned int nSocket )
{
	char szBuff[MaxBuffLen];
	char szMethod[MaxPathLen];
	char szUrl[MaxPathLen];
	char szPath[MaxPathLen];
	int nNumChars;
	size_t i, j;
	struct stat st;
	bool bCgi = false;
	char* pQueryString = nullptr;	

	//get request first line
	nNumChars = GetLine(nSocket, szBuff, MaxBuffLen);
	i = j = 0;

	//get request method in to szMethod
	while( ! IsSpace(szBuff[i]) && (i < MaxPathLen - 1)){
		szMethod[i] = szBuff[j];
		i++;
		j++;
	}
	szMethod[i] = 0;

	//if no get or no post
	if (stricmp(szMethod, "get") && stricmp(szMethod, "post")){
		Unimplemented(nSocket);
		return;
	}

	//enable cgi if post
	if (stricmp(szMethod, "post") == 0){
		bCgi = true;
	}

	//load url
	i = 0;
	while(IsSpace(szBuff[j]) && (j < MaxBuffLen))
		j++;//skip space char
	while( ! IsSpace(szBuff[j]) && (i < (MaxPathLen - 1)) && (j < MaxBuffLen)){
		szUrl[i] = szBuff[j];
		i++;
		j++;
	}
	szUrl[i] = 0;

	//deal get method
	if (stricmp(szMethod, "get") == 0){
		pQueryString = szUrl;
		while((*pQueryString != '?') && (*pQueryString != 0))
			pQueryString++;

		if (*pQueryString == '?'){// has param
			bCgi = true;
			*pQueryString = 0;
			pQueryString++;
		}
	}

	// format szUrl into szPath, all html files in htdocs
	pFuncOutput("Request", szUrl);
	
	if (szUrl[0] == '/' && szUrl[1] == 0){
		sprintf(szUrl, "/index.html");
	}

	sprintf(szPath, "%s%s", szAppPath, szUrl);

	if (stat(szPath, &st) == -1){
		//clear headers
		while((nNumChars > 0) && stricmp("\n", szBuff)){
			nNumChars = GetLine(nSocket, szBuff, MaxBuffLen);
		}
		NotFound(nSocket);
	}else{
		if ((st.st_mode & S_IFMT) == S_IFDIR){
			strcat(szPath, "/index.html");			
		}
		//if execute_cgi
		if ( ! bCgi){
			ServerFile(nSocket, szPath);
		}else{
			ExecuteCgi(nSocket, szPath, szMethod, pQueryString);
		}
	}
	Close(nSocket);
}

void TinyHttpD::BadRequest( int nSocket )
{
	char szBuff[MaxBuffLen];
	sprintf(szBuff, "HTTP/1.0 400 BAD REQUEST\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"<P>Your browser sent a bad request, such as a POST without a Content-Length.\r\n");
	send(nSocket, szBuff, strlen(szBuff), 0);
	char szSocket[16];
	sprintf(szSocket, "Send(%d)", nSocket);
	pFuncOutput(szSocket, szBuff);
	send(nSocket, szBuff, strlen(szBuff), 0);
}

void TinyHttpD::Cat( int nSocket, FILE* pResource )
{
	char szBuff[MaxBuffLen];
	if (pResource != (FILE*)NULL){
		while(fgets(szBuff, MaxBuffLen, pResource)){
			char szSocket[16];
			sprintf(szSocket, "Send(%d)", nSocket);
			pFuncOutput(szSocket, szBuff);
			send(nSocket, szBuff, strlen(szBuff), 0);
		}
	}
}

void TinyHttpD::CannotExecute( int nSocket )
{
	char szBuff[MaxBuffLen];
	sprintf(szBuff, "HTTP/1.0 500 Internal Server Error\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"<P>Error prohibited CGI execution.\r\n");
	char szSocket[16];
	sprintf(szSocket, "Send(%d)", nSocket);
	pFuncOutput(szSocket, szBuff);
	send(nSocket, szBuff, strlen(szBuff), 0);
}

void TinyHttpD::ErrorDie( char *szError )
{
	pFuncOutput("ErrorDie", szError);
	exit(1);
}

void TinyHttpD::SendHeader( unsigned int nSocket, char* pFileName )
{
	char szBuff[MaxBuffLen];
	sprintf(szBuff, "HTTP/1.0 200 OK\r\n"
		SERVER_STRING
		"Content-Type: text/html\r\n"
		"\r\n");
	send(nSocket, szBuff, strlen(szBuff), 0);
}

class CThreadData{
public:
	struct sockaddr_in clientName;
	TinyHttpD* pHttpd;
	unsigned int nSocket;
};

int ListenThread(void* pParam){
	CThreadData* pData = (CThreadData*)pParam;
	pData->pHttpd->DealRequest(pData->nSocket);
	return 0;
};

int AcceptThread(void* pParam){
	TinyHttpD* pHttpd = (TinyHttpD*)pParam;
	int nNameLen = sizeof(sockaddr_in);
	while(1){
		CThreadData* pData = new CThreadData;
		pData->pHttpd = pHttpd;
		pData->nSocket = accept(pHttpd->GetHttpd(), (struct sockaddr*)&pData->clientName, &nNameLen);
		if (pData->nSocket == -1){
			pHttpd->ErrorDie("AcceptError");
		}else{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ListenThread, pData, 0, 0);
		}
	}
	return 0;
};

void TinyHttpD::Start( int& nPort )
{
	SOCKET httpd = 0;
	struct sockaddr_in name;
	httpd = socket(AF_INET, SOCK_STREAM, 0);
	if (httpd == INVALID_SOCKET){
		ErrorDie("ErrorSocket");
	}
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(nPort);
	name.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(httpd, (struct sockaddr*)&name, sizeof(name)) < 0){
		ErrorDie("BindError");
	}
	if (nPort == 0){
		int nNameLen = sizeof(name);
		if (getsockname(httpd, (struct sockaddr*)&name, &nNameLen) == -1){
			ErrorDie("GetSockNameError");
		}
		nPort = ntohs(name.sin_port);
	}
	if(listen(httpd, 5) < 0){
		ErrorDie("ListenError");
	}
	char sz[128];
	sprintf(sz, "Server-Start, port: [%d], nSocket: [%d]", nPort, httpd);
	pFuncOutput("Start", sz);
	nHttpd = httpd;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AcceptThread, this, 0, 0);
}

unsigned int TinyHttpD::GetHttpd()
{
	return nHttpd;
}

TinyHttpD::TinyHttpD(pOutput pFuncOutput)
{
	WSAData wsaData;
	WSAStartup(0x0202, &wsaData);
	GetModuleFileNameA(nullptr, szAppPath, 1024);
	char* p = strrchr(szAppPath, '\\');
	*p = 0;

	this->pFuncOutput = pFuncOutput == nullptr ? DefaultOutput : pFuncOutput;
}

TinyHttpD::~TinyHttpD()
{

}

void TinyHttpD::DefaultOutput( char* pAction, char* pMsg )
{
	printf("[%s] : [%s]\n", pAction, pMsg);
}

void TinyHttpD::SetOutputFunction( pOutput pFuncOutput )
{
	if (pFuncOutput == nullptr){
		this->pFuncOutput = DefaultOutput;
	}else{
		this->pFuncOutput = pFuncOutput;
	}
}

int main(){
	TinyHttpD td;
	int nPort = 1000;
	td.Start(nPort);
	while(1){
		Sleep(1000);
	}
}

static TinyHttpD td;

TINYHTTPD_API int Start( int nPort, pOutput pFunc )
{
	td.SetOutputFunction(pFunc);
	td.Start(nPort);
	return nPort;
}

TINYHTTPD_API int Stop()
{
	return 0;
}

