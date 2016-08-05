#ifndef __I_HOUSE_H__
#define __I_HOUSE_H__
#include <iostream>

#define SHOW printf("%s\n", __FUNCTION__);;

class IProduct
{
public:
	virtual void Show() = 0;
};

class ICatHouse
{
public:
	virtual IProduct* SellCat() = 0;
	virtual IProduct* SellFood() = 0;
};

#endif