#include "StringMgr.h"

StringMgr::StringMgr(){
	pIter = nullptr;
};

void StringMgr::Push(const string& strValue){
	vec.push_back(strValue);
};

string StringMgr::Get(int n){
	return vec[n];
};

int StringMgr::Size(){
	return vec.size();
};

Iterator* StringMgr::GetIter(){
	if (pIter == nullptr)
	{
		pIter = new StringMgrIter(this);
	}
	return pIter;
};