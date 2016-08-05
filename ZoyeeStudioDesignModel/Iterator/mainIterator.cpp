#include "Iterator.h"
#include "StringMgr.h"

int main()
{
	StringMgr mgr;
	mgr.Push("Hello");
	mgr.Push("World");
	mgr.Push("Fuck");
	mgr.Push("Saigai!");

	Iterator* iter = mgr.GetIter();
	do
	{
		printf("[%s]\n", iter->Get().c_str());
	}while(iter->Next());
	
}