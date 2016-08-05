#ifndef __I_BUILDER_H__
#define __I_BUILDER_H__
#define SHOW printf("%s\n", __FUNCTION__);;
#include <iostream>

class CatBaby;
class IBuilder
{
public:
	virtual CatBaby* GetBaby() = 0;
	virtual void Create() = 0;
protected:
	virtual void BuildHead() = 0;
	virtual void BuildBody() = 0;
	virtual void BuildLeg() = 0;
};



#endif