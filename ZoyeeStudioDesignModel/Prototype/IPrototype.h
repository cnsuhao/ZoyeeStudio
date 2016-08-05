#ifndef __PROTOTYPE_H__
#define __PROTOTYPE_H__

#include <iostream>

class IPrototype
{
public:
	virtual IPrototype* Clone() = 0;
	~IPrototype(void){};
	virtual void Show(){};

protected:
	IPrototype(void){};
};

#endif