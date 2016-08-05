#include "Iterator.h"
#include "StringMgr.h"

	StringMgrIter::StringMgrIter(StringMgr* pMgr) : Iterator(pMgr){
		Reset();
	};

	void StringMgrIter::Reset(){
		nCurrnt = 0;
	};

	string StringMgrIter::Get(){
		return pMgr->Get(nCurrnt);
	};

	bool StringMgrIter::Next(){
		if ((nCurrnt + 1) >= pMgr->Size())
		{
			return false;
		}
		else
		{
			nCurrnt++;
			return true;
		}
	};