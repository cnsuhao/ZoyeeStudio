#ifndef __MOUSE_TOOL_H__
#define __MOUSE_TOOL_H__

#include <iostream>

class MouseTool
{
public:
	virtual void CatchMouse(){
		printf("%s\n", __FUNCTION__);
	}
};

class Cat
{
public:
	virtual void CatchMouse(){
		printf("%s\n", __FUNCTION__);
	}
};

class Tool : public MouseTool
{
public:
	Tool(Cat* pCat = nullptr){
		this->pCat = pCat;
	};

	virtual void CatchMouse(){
		if (pCat)
		{
			pCat->CatchMouse();
		}
		else
			MouseTool::CatchMouse();
	}
protected:
	Cat* pCat;
};

#endif