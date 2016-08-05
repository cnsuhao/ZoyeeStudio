#ifndef __I_BASE_H__
#define __I_BASE_H__

#include <iostream>

class ICat;
class ICatHouse;

class ICat
{
public:
	virtual void Action() = 0;
};

class ICatHouse
{
public:
	virtual ICat* Sell() = 0;
};



#endif