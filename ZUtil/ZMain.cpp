#include "ZUtils.h"

int main(){
	char* pGuid = ZUtil::NewGuid();
	while(1)
	{
		Sleep(1000);
		printf("%s\n", ZUtil::NewGuid());
	}
	
}