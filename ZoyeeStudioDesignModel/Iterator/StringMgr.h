#ifndef __STRING_MGR_H__
#define __STRING_MGR_H__

#include "Iterator.h"
#include <vector>

using namespace std;

class StringMgr
{
public:
	StringMgr();

	void Push(const string& strValue);
	string Get(int n);
	int Size();
	Iterator* GetIter();

protected:
	vector<string> vec;
	Iterator* pIter;
};

#endif