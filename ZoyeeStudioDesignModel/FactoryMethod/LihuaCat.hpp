#ifndef __LIHUA_CAT_H__
#define __LIHUA_CAT_H__

#include "IBase.hpp"

class LihuaCat;

class LihuaCat : public ICat
{
public:
	void Action(){
		printf("%s\n", __FUNCTION__);
	};
};

class LihuaCatHouse : public ICatHouse
{
public:
	LihuaCat* Sell(){
		return new LihuaCat;
	};
};

#endif