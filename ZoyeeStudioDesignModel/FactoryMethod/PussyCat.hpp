#ifndef __PUSSY_CAT_H__
#define __PUSSY_CAT_H__

#include "IBase.hpp"
class PussyCat;

class PussyCat : public ICat
{
public:
	void Action(){
		printf("%s\n", __FUNCTION__);
	};
};

class PussyCatHouse : public ICatHouse
{
public:
	PussyCat* Sell(){
		return new PussyCat;
	}
};

#endif