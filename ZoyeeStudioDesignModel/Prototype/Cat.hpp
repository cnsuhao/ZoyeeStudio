#ifndef __CAT_H__
#define __CAT_H__

#include "IPrototype.h"

class Cat : public IPrototype
{
public:
	Cat(){
		nId = 0;
	};

	Cat(const Cat& cat){
		nId = cat.nId;
	};

	IPrototype* Clone(){
		return new Cat(*this);
	};

	void Show(){
		printf("[%p]id: %d", this, nId);
	};
protected:
	int nId;
};

#endif