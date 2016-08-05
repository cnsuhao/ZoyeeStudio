#ifndef __CAT_H__
#define __CAT_H__

#include <iostream>

class CatGod
{
public:
	static CatGod* GetInstance(){
		static CatGod god;
		return &god;
	};

	void Show(){
		printf("[%p]\n", this);
	}
};


#endif