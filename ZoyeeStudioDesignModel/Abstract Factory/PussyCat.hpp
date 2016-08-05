#ifndef __PUSSY_CAT_H__
#define __PUSSY_CAT_H__

#include "IHouse.h"

class PussyCat : public IProduct
{
public:
	void Show(){
		SHOW;
	};
};

class PussyCatFood : public IProduct
{
public:
	void Show(){SHOW;};
};

class PussyCatHouse : public ICatHouse
{
public:
	IProduct* SellCat(){
		return new PussyCat;
	};

	IProduct* SellFood(){
		return new PussyCatFood;
	}
};

#endif