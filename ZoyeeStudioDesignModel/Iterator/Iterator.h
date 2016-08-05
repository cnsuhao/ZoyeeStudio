#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <string>
class StringMgr;

using namespace std;

class Iterator
{
public:	
	Iterator(StringMgr* pMgr){
		this->pMgr = pMgr;
	};
	virtual void Reset() = 0;
	virtual string Get() = 0;
	virtual bool Next() = 0;
protected:
	StringMgr * pMgr;
};

class StringMgrIter : public Iterator
{
public:
	StringMgrIter(StringMgr* pMgr);

	virtual void Reset();

	virtual string Get();

	virtual bool Next();

protected:
	int nCurrnt;
};

#endif