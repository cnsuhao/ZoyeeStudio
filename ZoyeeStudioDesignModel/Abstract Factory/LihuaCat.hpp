#ifndef __LIHUA_CAT_H__
#define __LIHUA_CAT_H__

#include "IHouse.h"

class LihuaCat : public IProduct
{
public:
	void Show(){
		SHOW;
	};
};

class LihuaCatFood : public IProduct
{
public:
	void Show(){
		SHOW;
	}
};

class LihuaCatHouse : public ICatHouse
{
public:
	IProduct* SellCat(){
		return new LihuaCat;
	};

	IProduct* SellFood(){
		return new LihuaCatFood;
	}
};

#endif