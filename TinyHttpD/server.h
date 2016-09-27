
#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>

typedef void(*pOutput)(char* pAction, char* pMsg);

class TinyHttpD{
public:
	TinyHttpD(pOutput pFuncOutput = nullptr);
	~TinyHttpD();
	void SetOutputFunction(pOutput pFuncOutput);
	void Start(int& nPort);
	void DealRequest(unsigned int nSocket);
	int GetLine(unsigned int nSocket, char* pBuffer, int nBuffSize);
	void Unimplemented(unsigned int nSocket);// no get and no post
	void NotFound(unsigned int nSocket);
	void ServerFile(unsigned int nSocket, char* pPath);
	void ExecuteCgi(unsigned int nSocket, char* pPath, char* pMethod, char* pQueryString);
	void Close(unsigned int nSocket);
	void BadRequest(int nSocket);
	void Cat(int nSocket, FILE* pResource);
	void CannotExecute(int nSocket);
	void ErrorDie(char *szError);
	void SendHeader(unsigned int nSocket, char* pFileName);
	unsigned int GetHttpd();
protected:
	static void DefaultOutput(char* pAction, char* pMsg);
	pOutput pFuncOutput;
	unsigned int nHttpd;
	//void Deal(int nSocket);
};

#endif